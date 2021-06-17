# Fallrim Priority [![GitHub](https://img.shields.io/github/license/BThree496/PriorityMod?style=flat-square&logo=github)](https://github.com/BThree496/PriorityMod/blob/main/LICENSE)

Ensured the game **always take most of CPU usage, prevent suddenly lag caused by other processes**.  
Kick other processes out. Skyrim/Fallout is the only one who should have whole CPU.  

## Supported Games
- Skyrim  LE/SE/VR  
- Enderal LE/SE/VR  
- Fallout NV/3/4/VR  
- Oblivion


## Priority Levels
|     Priority | Value | 
|------------- |------ |
| Idle         | 0     |
| Below Normal | 1     |
| Normal       | 2     |
| Above Normal | 3     |
| High         | 4     |
| Realtime     | 5     |

> This mod uses `High` by default  
> You can change it in `PriorityMod.toml`

> Run as Administrator is required for `Realtime`

## Note
### About `Realtime` Priority
https://docs.microsoft.com/en-us/windows/win32/procthread/scheduling-priorities#priority-class
>You should almost never use `Realtime`, because this interrupts system threads that manage mouse input, keyboard input, and background disk flushing. This class can be appropriate for applications that "talk" directly to hardware or that perform brief tasks that should have limited interruptions.
