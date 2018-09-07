extern crate libc;

// La définition de cette structure doit être identique à celle de C
// L'intérêt de cette même définition permet d'avoir accès aux fonctions de parsing
// définies uniquement en C
// Afin de pouvoir communiquer entre Rust et C, il faut utiliser le crate
// [libc](https://docs.rs/libc/0.2.43/libc/index.html) et utiliser la même représentation mémoire
// qu'en C avec [repr(C)](https://doc.rust-lang.org/nomicon/other-reprs.html)
#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct Servo2019 {
    id: libc::uint8_t,
    position: libc::uint16_t,
    wanted_position: libc::uint16_t,
    speed: libc::uint8_t,
    blocked: libc::c_char,
    blocking_mode: libc::uint8_t,
    color: libc::uint8_t,
}

#[repr(C)]
#[derive(PartialEq, Debug, Copy, Clone)]
pub struct SharedServos2019 {
    servos: [Servo2019; 8],
    parsing_failed: libc::uint8_t,
}

impl PartialEq for Servo2019 {
    fn eq(&self, other: &Servo2019) -> bool {
        self.id == other.id && (self.id == 0 || (self.position == other.position && self.wanted_position == other.wanted_position && self.speed == other.speed && self.blocked == other.blocked && self.blocking_mode == other.blocking_mode && self.color == other.color))
    }
}
impl Eq for Servo2019 {}

extern "C" {
    fn servo_read_frame(message: *const libc::uint8_t, size: libc::uint8_t) -> SharedServos2019;
    fn servo_write_frame(buf: *mut libc::uint8_t, buf_size: libc::uint8_t, obj: *const SharedServos2019) -> libc::uint8_t;
}

// Interfaces safes qui utlisent le code C
pub fn safe_servo_write_frame(obj: &SharedServos2019) -> Option<Vec<u8>> {
    unsafe {
        let mut buf = Vec::with_capacity(255);
        let size = servo_write_frame(buf.as_mut_ptr(), buf.capacity() as libc::uint8_t, obj);
        if size == 0 {
            println!("Building SharedServos2019's frame failed: not enough space.");
            None
        } else {
            buf.set_len(size as usize);
            Some(buf)
        }
    }
}

pub fn safe_servo_read_frame(message: Vec<u8>) -> Option<SharedServos2019> {
    unsafe {
        let servo = servo_read_frame(message.as_ptr(), message.len() as libc::uint8_t);
        if servo.parsing_failed == 1 {
            None
        } else {
            Some(servo)
        }
    }
}

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

    let written_frame = safe_servo_write_frame(&struct_before);
    assert!(written_frame.is_some());
    let read_frame = safe_servo_read_frame(written_frame.unwrap());
    assert!(read_frame.is_some());
    let struct_after = read_frame.unwrap();
    assert_eq!(struct_before, struct_after);

    println!("[OK] read(write(servo)) == servo");
}
