mod common;

#[cfg(any(debug_assertions, all(feature = "fallout_4", target_arch = "x86_64")))]
mod fallout4;

#[cfg(any(debug_assertions, all(feature = "skyrim_ae", target_arch = "x86_64")))]
mod skyrim_ae;