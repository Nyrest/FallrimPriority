#[cfg(test)]
mod tests {
    use std::vec::Vec;
    use windows::{
        core::{PCSTR},
        Win32::System::{
            LibraryLoader::LoadLibraryA,
            Threading::{
                GetCurrentProcess, GetPriorityClass, SetPriorityClass, ABOVE_NORMAL_PRIORITY_CLASS,
                BELOW_NORMAL_PRIORITY_CLASS, HIGH_PRIORITY_CLASS, IDLE_PRIORITY_CLASS,
                NORMAL_PRIORITY_CLASS, REALTIME_PRIORITY_CLASS,
            },
        },
    };

    #[test]
    fn load_lib() {
        let process = unsafe { GetCurrentProcess() };
        unsafe { SetPriorityClass(process, NORMAL_PRIORITY_CLASS) };
        let mut priority_class = unsafe { GetPriorityClass(process) };
        assert_eq!(priority_class, NORMAL_PRIORITY_CLASS.0);

        unsafe {
            LoadLibraryA(PCSTR::from_raw("./FallrimPriority.dll".as_ptr())).unwrap();
        }

        priority_class = unsafe { GetPriorityClass(process) };
        assert_eq!(priority_class, HIGH_PRIORITY_CLASS.0);

        assert_eq!(4, 4);
    }
}
