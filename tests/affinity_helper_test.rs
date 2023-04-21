mod tests_affinity {
    use FallrimPriority::affinity_helper::{
        calc_best_affinity, get_mask_cpu0, get_mask_smt_first_processors,
    };

    // TODO: Implement tests of min_threads

    macro_rules! test_affinity {
        ($name:expr, $cores:expr, $threads:expr, $mask_smt_first_processors:expr, $mask_cpu0:expr) => {
            paste::item! {
                #[test]
                fn [<test_ $name _  $cores c $threads t>]() {
                    // Count of physical cores
                    const CORES: usize = $cores;
                    // Count of logical processors
                    const THREADS: usize = $threads;
                    // Mask of all logical processors
                    const MASK_FULL: usize = {
                        if THREADS >= 64 {
                            usize::MAX
                        } else {
                            (1 << THREADS) - 1
                        }
                    };
                    // Mask of first logical processors in all cores
                    const MASK_SMT_FIRST_PROCESSORS: usize = $mask_smt_first_processors;
                    // Mask of logical processors related to Core 0
                    const MASK_CPU0: usize = $mask_cpu0;

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

    // test_affinity!($name, $num_cores, $num_threads, $mask_smt, $mask_cpu0);
    // NOTE: $mask_cpu0 should mask all Logical Processors related to Core 0

    test_affinity!("intel_13900x", 24, 32, 0x0000000000555555, 0x3);

    test_affinity!("generic_smt", 64, 128, 0x5555555555555555, 0x3);
    test_affinity!("generic_smt", 32, 64, 0x5555555555555555, 0x3);
    test_affinity!("generic_smt", 28, 56, 0x5555555555555555, 0x3);
    test_affinity!("generic_smt", 24, 48, 0x5555555555555555, 0x3);
    test_affinity!("generic_smt", 16, 32, 0x5555555555555555, 0x3);
    test_affinity!("generic_smt", 12, 24, 0x5555555555555555, 0x3);
    test_affinity!("generic_smt", 8, 16, 0x5555555555555555, 0x3);
    test_affinity!("generic_smt", 4, 8, 0x5555555555555555, 0x3);

    test_affinity!("generic", 64, 64, 0xFFFFFFFFFFFFFFFF, 0x1);
    test_affinity!("generic", 32, 32, 0xFFFFFFFFFFFFFFFF, 0x1);
    test_affinity!("generic", 28, 28, 0xFFFFFFFFFFFFFFFF, 0x1);
    test_affinity!("generic", 24, 24, 0xFFFFFFFFFFFFFFFF, 0x1);
    test_affinity!("generic", 16, 16, 0xFFFFFFFFFFFFFFFF, 0x1);
    test_affinity!("generic", 12, 12, 0xFFFFFFFFFFFFFFFF, 0x1);
    test_affinity!("generic", 8, 8, 0xFFFFFFFFFFFFFFFF, 0x1);
    test_affinity!("generic", 6, 6, 0xFFFFFFFFFFFFFFFF, 0x1);
    test_affinity!("generic", 4, 4, 0xFFFFFFFFFFFFFFFF, 0x1);
    test_affinity!("generic", 2, 2, 0xFFFFFFFFFFFFFFFF, 0x1);

    #[test]
    #[allow(non_snake_case)]
    fn test_this_cpu() {
        let CORES: usize = num_cpus::get_physical();
        let THREADS: usize = num_cpus::get();
        let MASK_FULL: usize = {
            if THREADS >= 64 {
                usize::MAX
            } else {
                (1 << THREADS) - 1
            }
        };
        let MASK_SMT_FIRST_PROCESSORS: usize = get_mask_smt_first_processors(); // All odd logical processors
        let MASK_CPU0: usize = get_mask_cpu0();

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
