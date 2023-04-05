use std::ffi::c_char;
pub type PluginHandle = u32;

#[repr(C)]
#[allow(non_snake_case)]
pub struct CommonPluginInfo {
    pub infoVersion: u32,
    pub name: *mut c_char,
    pub version: u32,
}
