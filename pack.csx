#r "System.IO.Compression"
using System;
using System.IO;
using System.IO.Compression;
using System.Text;
using System.Threading.Tasks;
using static Arch;

const string ModReleasePath = ".\\ModRelease";
const string ModDllName = "PriorityMod.dll";
const string ModTomlName = "PriorityMod.toml";

ModReleaseInfo[] outputs = new ModReleaseInfo[]
{
    new ("Skyrim Priority LE" ,"SKSE", x86), // The Elder Scroll Skyrim SE/VR
    new ("Skyrim Priority SE" ,"SKSE", x86_64), // The Elder Scroll Skyrim LE
    new ("Enderal Priority LE" ,"SKSE", x86), // Enderal LE
    new ("Enderal Priority SE" ,"SKSE", x86_64), // Enderal SE
    new ("Oblivion Priority" ,"OBSE", x86), // The Elder Scroll Oblivion
    new ("Fallout4 Priority" ,"F4SE", x86_64), // Fallout 4/VR
    new ("Fallout3 Priority" ,"FOSE", x86), // Fallout 3
    new ("NewVegas Priority" ,"NVSE", x86), // Fallout New Vegas
    //new ("Redfall Priority" ,"RESE", x86_64), // The Elder Scroll Redfall
};

byte[] dll32 = File.ReadAllBytes($"{GetCompiledPath(x86)}\\{ModDllName}");
byte[] dll64 = File.ReadAllBytes($"{GetCompiledPath(x86_64)}\\{ModDllName}");
byte[] toml = File.ReadAllBytes($"{GetCompiledPath(x86_64)}\\{ModTomlName}");

if (!Directory.Exists(ModReleasePath)) Directory.CreateDirectory(ModReleasePath);

Parallel.ForEach(outputs, releaseInfo =>
{
    using FileStream fs = new($"{ModReleasePath}\\{releaseInfo.modName}.zip", FileMode.Create, FileAccess.Write);
    using ZipArchive zipArchive = new ZipArchive(fs, ZipArchiveMode.Create, false, Encoding.UTF8);

    using (Stream dllStream = zipArchive.CreateEntry(GetEntryFullName("PriorityMod.dll"), CompressionLevel.Optimal).Open())
    {
        var dll = releaseInfo.arch switch
        {
            x86 => dll32,
            x86_64 => dll64,
            _ => throw new Exception("Unexpected architecture"),
        };
        dllStream.Write(dll, 0, dll.Length);
    }

    using (Stream tomlStream = zipArchive.CreateEntry(GetEntryFullName("PriorityMod.toml"), CompressionLevel.Optimal).Open())
    {
        tomlStream.Write(toml, 0, toml.Length);
    }

    Console.WriteLine($"[{releaseInfo.arch.ToString()}] {releaseInfo.modName}.zip");

    string GetEntryFullName(string name)
    {
        return $"Data\\{releaseInfo.seDirName}\\Plugins\\{name}";
    }
});

string GetCompiledPath(Arch arch)
{
    return arch switch
    {
        x86 => ".\\Release\\",
        x86_64 => ".\\x64\\Release\\",
        _ => throw new Exception("Unexpected architecture"),
    };
}

enum Arch
{
    x86, x86_64
}
record ModReleaseInfo(string modName, string seDirName, Arch arch);
