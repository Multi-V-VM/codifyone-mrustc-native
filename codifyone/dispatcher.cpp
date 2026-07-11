#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <vector>

extern int mrustc_main(int argc, char* argv[]);
extern int minicargo_main(int argc, const char* argv[]);

static bool has_arg(int argc, char** argv, const char* needle) {
    for (int i = 0; i < argc; ++i) {
        if (std::strcmp(argv[i], needle) == 0) return true;
    }
    return false;
}

static bool is_directory(const char* path) {
    struct stat value {};
    return stat(path, &value) == 0 && S_ISDIR(value.st_mode);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "CodifyOne Rust tools: expected rustc, cargo, or rustup\n";
        return 2;
    }
    const std::string tool = argv[1];
    std::vector<char*> forwarded;
    forwarded.push_back(argv[1]);
    for (int i = 2; i < argc; ++i) forwarded.push_back(argv[i]);

    if (tool == "rustup") {
        if (forwarded.size() == 1 || (forwarded.size() > 1 && std::strcmp(forwarded[1], "show") == 0)) {
            std::cout << "Default host: wasm32-wasip1\n"
                         "active toolchain\n"
                         "----------------\n"
                         "codifyone-mrustc-1.90 (default)\n";
            return 0;
        }
        if (forwarded.size() > 1 && std::strcmp(forwarded[1], "target") == 0) {
            std::cout << "wasm32-wasip1 (installed)\n";
            return 0;
        }
        if (forwarded.size() > 1 && (std::strcmp(forwarded[1], "--version") == 0 || std::strcmp(forwarded[1], "-V") == 0)) {
            std::cout << "rustup codifyone-wasi experimental\n";
            return 0;
        }
        std::cerr << "rustup: this offline experiment only supports show, target list, and --version\n";
        return 1;
    }

    const char* spec = std::getenv("CODIFYONE_RUST_TARGET_SPEC");
    const char* libdir = std::getenv("CODIFYONE_RUST_LIBDIR");
    if (tool == "rustc") {
        // mrustc assumes an input exists and aborts through C++ exception code
        // when invoked bare. Wasmi reports that abort as an `unreachable` trap.
        if (forwarded.size() == 1) {
            std::cerr << "usage: rustc [OPTIONS] INPUT\n";
            return 1;
        }
        if (spec && !has_arg(static_cast<int>(forwarded.size()), forwarded.data(), "--target")) {
            forwarded.push_back(const_cast<char*>("--target"));
            forwarded.push_back(const_cast<char*>(spec));
        }
        if (libdir) {
            forwarded.push_back(const_cast<char*>("-L"));
            forwarded.push_back(const_cast<char*>(libdir));
        }
        forwarded.push_back(nullptr);
        setenv("MRUSTC_TARGET_VER", "1.90", 1);
        return mrustc_main(static_cast<int>(forwarded.size() - 1), forwarded.data());
    }
    if (tool == "cargo") {
        if (forwarded.size() == 1) {
            std::cerr << "usage: cargo build [PACKAGE_DIR]\n       cargo [PACKAGE_DIR]\n";
            return 1;
        }
        if (std::strcmp(forwarded[1], "--version") == 0 || std::strcmp(forwarded[1], "-V") == 0) {
            std::cout << "cargo codifyone-minicargo 1.90 (wasm32-wasip1)\n";
            return 0;
        }
        if (std::strcmp(forwarded[1], "build") == 0) {
            forwarded.erase(forwarded.begin() + 1);
            if (forwarded.size() == 1) forwarded.push_back(const_cast<char*>("."));
        }
        // minicargo's missing-manifest path throws; validate the package before
        // entering it so a CLI mistake remains a normal nonzero exit.
        if (forwarded.size() > 1 && !is_directory(forwarded[1])) {
            std::cerr << "cargo: package path is not a directory: " << forwarded[1] << "\n";
            return 1;
        }
        forwarded.push_back(const_cast<char*>("-j"));
        forwarded.push_back(const_cast<char*>("1"));
        if (spec && !has_arg(static_cast<int>(forwarded.size()), forwarded.data(), "--target")) {
            forwarded.push_back(const_cast<char*>("--target"));
            forwarded.push_back(const_cast<char*>(spec));
        }
        if (libdir) {
            forwarded.push_back(const_cast<char*>("-L"));
            forwarded.push_back(const_cast<char*>(libdir));
        }
        forwarded.push_back(nullptr);
        setenv("MRUSTC_TARGET_VER", "1.90", 1);
        return minicargo_main(static_cast<int>(forwarded.size() - 1), const_cast<const char**>(forwarded.data()));
    }
    std::cerr << "Unknown CodifyOne Rust tool: " << tool << "\n";
    return 2;
}
