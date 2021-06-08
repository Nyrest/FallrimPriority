# Skyrim/Fallout Priority [![GitHub](https://img.shields.io/github/license/BThree496/PriorityMod?style=flat-square&logo=github)](https://github.com/BThree496/PriorityMod/blob/main/LICENSE)

Ensured the game **always take most of CPU usage, prevent suddenly lag caused by other processes**.  
Kick other processes out. Skyrim/Fallout is the only one who should have whole CPU.  

## Priority Classes
|     Priority | Value | 
|------------- |------ |
| Idle         | 64    |
| Below Normal | 16384 |
| Normal       | 32    |
| Above Normal | 32768 |
| High         | 128   |
| Realtime     | 256   |

> This mod uses `High` by default  
> You can change it in `PriorityMod.toml`

> Run as Administrator is required for `Realtime`

## Note
### About `Realtime` Priority
https://docs.microsoft.com/en-us/windows/win32/procthread/scheduling-priorities#priority-class
>You should almost never use `Realtime`, because this interrupts system threads that manage mouse input, keyboard input, and background disk flushing. This class can be appropriate for applications that "talk" directly to hardware or that perform brief tasks that should have limited interruptions.
