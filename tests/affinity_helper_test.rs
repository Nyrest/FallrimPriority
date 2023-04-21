mod tests_affinity {
    use FallrimPriority::affinity_helper::{
        calc_best_affinity, get_mask_cpu0, get_mask_smt_first_processors,
    };

    use crate::{test_affinity, test_affinity_generic};

    // TODO: Implement tests of min_threads

    // test_affinity!($name, $num_cores, $num_threads, $mask_smt, $mask_cpu0);
    // NOTE: $mask_cpu0 should mask all Logical Processors related to Core 0

    test_affinity!("intel_13900k", 24, 32, 0x0000000000555555, 0x3);
    test_affinity!("intel_13700k", 16, 24, 0x0000000000005555, 0x3);
    test_affinity!(
        "this_cpu",
        num_cpus::get_physical(),
        num_cpus::get(),
        get_mask_smt_first_processors(),
        get_mask_cpu0()
    );

    test_affinity_generic!("smt", 64, 128, 0x5555555555555555, 0x3);
    test_affinity_generic!("smt", 48, 96, 0x5555555555555555, 0x3);
    test_affinity_generic!("smt", 32, 64, 0x5555555555555555, 0x3);
    test_affinity_generic!("smt", 28, 56, 0x5555555555555555, 0x3);
    test_affinity_generic!("smt", 24, 48, 0x5555555555555555, 0x3);
    test_affinity_generic!("smt", 16, 32, 0x5555555555555555, 0x3);
    test_affinity_generic!("smt", 12, 24, 0x5555555555555555, 0x3);
    test_affinity_generic!("smt", 8, 16, 0x5555555555555555, 0x3);
    test_affinity_generic!("smt", 4, 8, 0x5555555555555555, 0x3);

    test_affinity_generic!("non_smt", 64, 64, 0xFFFFFFFFFFFFFFFF, 0x1);
    test_affinity_generic!("smt", 48, 48, 0xFFFFFFFFFFFFFFFF, 0x1);
    test_affinity_generic!("non_smt", 32, 32, 0xFFFFFFFFFFFFFFFF, 0x1);
    test_affinity_generic!("non_smt", 28, 28, 0xFFFFFFFFFFFFFFFF, 0x1);
    test_affinity_generic!("non_smt", 24, 24, 0xFFFFFFFFFFFFFFFF, 0x1);
    test_affinity_generic!("non_smt", 16, 16, 0xFFFFFFFFFFFFFFFF, 0x1);
    test_affinity_generic!("non_smt", 12, 12, 0xFFFFFFFFFFFFFFFF, 0x1);
    test_affinity_generic!("non_smt", 8, 8, 0xFFFFFFFFFFFFFFFF, 0x1);
    test_affinity_generic!("non_smt", 6, 6, 0xFFFFFFFFFFFFFFFF, 0x1);
    test_affinity_generic!("non_smt", 4, 4, 0xFFFFFFFFFFFFFFFF, 0x1);
    test_affinity_generic!("non_smt", 2, 2, 0xFFFFFFFFFFFFFFFF, 0x1);
}

mod test_affinity_macros {
    #[macro_export]
    macro_rules! test_affinity {
        ($name:expr, $cores:expr, $threads:expr, $mask_smt_first_processors:expr, $mask_cpu0:expr) => {
            paste::item! {
                #[test]
                #[allow(non_snake_case)]
                fn [<test_ $name>]() {
                    // Count of physical cores
                    let CORES: usize = $cores;
                    // Count of logical processors
                    let THREADS: usize = $threads;
                    // Mask of all logical processors
                    let MASK_FULL: usize = {
                        if THREADS >= 64 {
                            usize::MAX
                        } else {
                            (1 << THREADS) - 1
                        }
                    };
                    // Mask of first logical processors in all cores
                    let MASK_SMT_FIRST_PROCESSORS: usize = $mask_smt_first_processors;
                    // Mask of logical processors related to Core 0
                    let MASK_CPU0: usize = $mask_cpu0;

                    // disable_cpu0 = false, disable_smt = false, min_threads = 6
                    let affinity = calc_best_affinity(
                        CORES,
                        THREADS,
                        MASK_SMT_FIRST_PROCESSORS,
                        MASK_CPU0,
                        false,
                        false,
                        6,
                    );
                    assert_eq!(affinity, MASK_FULL);

                    // disable_cpu0 = false, disable_smt = true, min_threads = 6
                    let affinity = calc_best_affinity(
                        CORES,
                        THREADS,
                        MASK_SMT_FIRST_PROCESSORS,
                        MASK_CPU0,
                        false,
                        true,
                        6,
                    );
                    assert_eq!(affinity, MASK_FULL & MASK_SMT_FIRST_PROCESSORS);

                    // disable_cpu0 = true, disable_smt = false, min_threads = 6
                    let affinity = calc_best_affinity(
                        CORES,
                        THREADS,
                        MASK_SMT_FIRST_PROCESSORS,
                        MASK_CPU0,
                        true,
                        false,
                        6,
                    );
                    assert_eq!(affinity, MASK_FULL & !MASK_CPU0);

                    // disable_cpu0 = true, disable_smt = true, min_threads = 6
                    let affinity = calc_best_affinity(
                        CORES,
                        THREADS,
                        MASK_SMT_FIRST_PROCESSORS,
                        MASK_CPU0,
                        true,
                        true,
                        6,
                    );
                    assert_eq!(affinity, MASK_FULL & MASK_SMT_FIRST_PROCESSORS & !MASK_CPU0);
                }
            }
        };
    }

    #[macro_export]
    macro_rules! test_affinity_generic {
        ($name_postfix:expr, $cores:expr, $threads:expr, $mask_smt_first_processors:expr, $mask_cpu0:expr) => {
            paste::item! {
                test_affinity!([<generic_ $name_postfix _  $cores c $threads t>], $cores, $threads, $mask_smt_first_processors, $mask_cpu0);
            }
        };
    }
}
