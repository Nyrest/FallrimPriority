use super::common::{CommonPluginInfo, PluginHandle};
use std::ffi::{c_void, CString};

#[repr(C)]
#[allow(non_snake_case)]
pub struct F4SEInterface {
    pub f4seVersion: u32,
    pub runtimeVersion: u32,
    pub editorVersion: u32,
    pub isEditor: u32,

    pub QueryInterface: extern "C" fn(u32) -> *const c_void,
    pub GetPluginHandle: extern "C" fn() -> PluginHandle,
    pub GetReleaseIndex: extern "C" fn() -> u32,
}

#[no_mangle]
pub unsafe extern "C" fn F4SEPlugin_Query(
    interface: *const F4SEInterface,
    info: *mut CommonPluginInfo,
) -> bool {
    (*info).infoVersion = 1;
    (*info).name = CString::new("Fallout Priority")
        .expect("could not create CString")
        .into_raw();
    (*info).version = 1;

    return (*interface).isEditor == 0;
}

#[no_mangle]
pub unsafe extern "C" fn F4SEPlugin_Load(interface: *const F4SEInterface) -> bool {
    true
}
