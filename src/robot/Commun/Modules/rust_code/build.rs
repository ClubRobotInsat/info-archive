extern crate cc;

fn main() {
    cc::Build::new()
        .file("../SharedWithRust.c")
        .compile("libSharedWithRust.a");
}
