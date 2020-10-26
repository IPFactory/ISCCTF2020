use nix::sys::ptrace;
use std::process::exit;
use crypto_hash::{hex_digest, Algorithm};

fn main() {
    detect();

    let args = std::env::args().collect::<Vec<String>>();

    if args.len() != 2  {
        println!("Usage: {} string", args[0]);
        exit(0);
    }


    if check_flag(&args[1]) {
        println!("{}", String::from_utf8(base64::decode(vec![70,67,91,81,68,36,71,82,112,38,87,37,118,120,83,127,79,67,44,127,91,82,32,115,91,82,109,111,88,83,44,124,88,36,44,36,91,65,91,126,77,38,68,98,77,39,68,111,113,81,91,127,113,83,44,126,88,39,95,36,79,39,118,109,119,120,113,44].iter().map(|x| x ^ 0x15).collect::<Vec<u8>>()).unwrap()).unwrap());
    }
}

fn check_flag(input: &str) -> bool {
    let enc = hex_digest(Algorithm::SHA256, &input.as_bytes());
    return enc == "81e5b17a265f06088905a692874cbd05faec0a85da61fb02a4251ef220648d82";
}

fn detect() {
    if let Err(_) = ptrace::traceme() {
        eprintln!("Detect debugger!!");
        exit(1); 
    }
}
