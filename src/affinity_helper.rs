use std::ptr::null_mut;

use windows::Win32::System::SystemInformation::{
    GetLogicalProcessorInformationEx, RelationProcessorCore,
    SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX,
};

pub fn calc_best_affinity(disabled_cpu0: bool, disable_smt: bool, min_threads: u32) -> usize {
    let num_cores = num_cpus::get_physical();
    let num_logical_processors = num_cpus::get();
    let smt_enabled = num_cores != num_logical_processors;
    let mut result: usize = {
        if num_logical_processors >= usize::MAX {
            usize::MAX
        } else {
            (1 << num_logical_processors) - 1
        }
    };

    if disabled_cpu0 {
        result &= !(1 << 0);
    }

    if smt_enabled && disable_smt {
        let mask = usize::MAX / 3;
        result &= mask;
    }

    if smt_enabled && disable_smt {
        result &= get_smt_mask();
    }

    // TODO: Implement min_threads

    return result;
}

fn get_smt_mask() -> usize {
    let mut result = 0;

    // Get length and create buffer
    let mut len = 0;
    unsafe { GetLogicalProcessorInformationEx(RelationProcessorCore, Some(null_mut()), &mut len) };
    let mut buffer = vec![0u8; len as usize];

    let info = buffer.as_mut_ptr() as *mut SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX;
    unsafe { GetLogicalProcessorInformationEx(RelationProcessorCore, Some(info), &mut len) };

    let mut current = info;
    while (current as usize) < (info as usize) + (len as usize) {
        if unsafe { (*current).Relationship } == RelationProcessorCore {
            for i in 0..unsafe { (*current).Anonymous.Processor.GroupCount } {
                let mask = unsafe { (*current).Anonymous.Processor.GroupMask[i as usize].Mask };
                let smt_mask = mask & !(1 << mask.trailing_zeros());
                result |= smt_mask;
            }
        }
        current = ((current as usize) + unsafe { (*current).Size as usize }) as *mut _;
    }
    return result;
}
