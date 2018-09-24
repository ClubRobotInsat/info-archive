//#![no_std]
extern crate core;
extern crate libc;

mod structs;

use structs::*;

fn main() {
    {
        let servo_empty = Servo2019 {
            id: 0,
            position: 0,
            wanted_position: 0,
            speed: 0,
            blocked: 0,
            blocking_mode: 0,
            color: 0,
        };
        assert_eq!(
            servo_empty,
            Servo2019 {
                id: 0,
                position: 1,
                wanted_position: 2,
                speed: 3,
                blocked: 4,
                blocking_mode: 5,
                color: 6,
            }
        );
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

    {
        let controlled_empty = ControlledMotor2019 {
            id: 0,
            wanted_angle_position: 0,
            wanted_nb_turns: 0,
            finished: 0,
            new_command: 0,
        };
        assert_eq!(
            controlled_empty,
            ControlledMotor2019 {
                id: 0,
                wanted_angle_position: 1,
                wanted_nb_turns: 2,
                finished: 3,
                new_command: 4,
            }
        );
        let controlled_1 = ControlledMotor2019 {
            id: 1,
            wanted_angle_position: 213,
            wanted_nb_turns: 2,
            finished: 0,
            new_command: 1,
        };
        assert_eq!(controlled_1, controlled_1);
        let controlled_3 = ControlledMotor2019 {
            id: 3,
            wanted_angle_position: 12,
            wanted_nb_turns: 5,
            finished: 1,
            new_command: 0,
        };
        assert_ne!(controlled_1, controlled_3);

        let uncontrolled_empty = UncontrolledMotor2019 {
            id: 0,
            on_off: 0,
            rotation: 0,
        };
        assert_eq!(
            uncontrolled_empty,
            UncontrolledMotor2019 {
                id: 0,
                on_off: 1,
                rotation: 0,
            }
        );
        let uncontrolled_1 = UncontrolledMotor2019 {
            id: 1,
            on_off: 0,
            rotation: 0,
        };
        assert_eq!(uncontrolled_1, uncontrolled_1);

        let brushless_empty = Brushless2019 { id: 0, on_off: 0 };
        assert_eq!(brushless_empty, Brushless2019 { id: 0, on_off: 1 });
        let brushless_1 = Brushless2019 { id: 1, on_off: 0 };
        assert_eq!(brushless_1, brushless_1);
        let brushless_5 = Brushless2019 { id: 5, on_off: 0 };
        let brushless_6 = Brushless2019 { id: 6, on_off: 1 };

        let mut array_controlled = [controlled_empty; 8];
        array_controlled[1] = controlled_1;
        array_controlled[3] = controlled_3;

        let mut array_uncontrolled = [uncontrolled_empty; 8];
        array_uncontrolled[1] = uncontrolled_1;

        let mut array_brushless = [brushless_empty; 8];
        array_brushless[1] = brushless_1;
        array_brushless[5] = brushless_5;
        array_brushless[6] = brushless_6;

        let struct_before = SharedMotors2019 {
            controlled_motors: array_controlled,
            uncontrolled_motors: array_uncontrolled,
            brushless: array_brushless,

            parsing_failed: 0,
        };

        let written_frame = struct_before.write_frame();
        assert!(written_frame.is_ok());
        let read_frame = SharedMotors2019::read_frame(written_frame.unwrap_or(Vec::new()));
        assert!(read_frame.is_ok());
        let struct_after = read_frame.unwrap_or(struct_before);
        assert_eq!(struct_before, struct_after);

        println!("[OK] read(write(motor)) == motor");
    }
}
