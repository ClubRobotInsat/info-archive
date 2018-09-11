//#![no_std]
extern crate core;
extern crate libc;

mod structs;

use structs::*;

fn main() {
    let servo_empty = Servo2019 {
        id: 0,
        position: 0,
        wanted_position: 0,
        speed: 0,
        blocked: 0,
        blocking_mode: 0,
        color: 0,
    };
    assert_eq!(servo_empty, Servo2019 { id: 0, position: 1, wanted_position: 2, speed: 3, blocked: 4, blocking_mode: 5, color: 6 });
    let servo1 = Servo2019 {
        id: 1,
        position: 512,
        wanted_position: 162,
        speed: 124,
        blocked: 0,
        blocking_mode: 0,
        color: 5,
    };
    assert_eq!(servo1, servo1);
    let servo3 = Servo2019 {
        id: 3,
        position: 1000,
        wanted_position: 10,
        speed: 12,
        blocked: 1,
        blocking_mode: 1,
        color: 3,
    };
    assert_ne!(servo1, servo3);

    let mut array = [servo_empty; 8];
    array[1] = servo1;
    array[3] = servo3;

    let struct_before = SharedServos2019 {
        servos: array,
        parsing_failed: 0,
    };

    let written_frame = struct_before.write_frame();
    assert!(written_frame.is_ok());
    let read_frame = SharedServos2019::read_frame(written_frame.unwrap_or(Vec::new()));
    assert!(read_frame.is_ok());
    let struct_after = read_frame.unwrap_or(struct_before);
    assert_eq!(struct_before, struct_after);

    println!("[OK] read(write(servo)) == servo");
}
