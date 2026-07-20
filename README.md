<div align="center">
  
<table>
<tr>
<td>  
<pre>
  <div align="left" >
　　　　　　　　　　　　　　　　　　＿＿＿＿_  
　　　　　　　　　　　　　　　　 ／::r―-ｖ―-､ヽ、  
　　　　　　　　　　　　　　　{!/:::/｀  
　　　　　　　　　　　　　　／:::::::l、＿　 　 ,ｨ　　 ＿,ｨ  
　　　　　　　　 　 _,...::'::´ヽ:::::::::〈´:::::_二二:ニ=-一'′  
　　　　　　 ,....::'´::::|::::.　.::::{_::::::rｲ￣  
　　,.ｨｸ=／;::::'´　 .:ヽ::::::::::::::厂 l{  
_,.ｨｸ´／／::::::::::::::::::_冫､_／　　ヽ、  
　　/:／:::::::::::::::_;::-ｲ!　　ヾ:、  
　　ヽ::::::::;;::‐'ｨｸ　　l:|　　　ヾ:、  
　　　 ￣　 〃　 　 l:|　　 　 ｀ヾ:、  
　　　　　 .〃 　 　 |:!　　　　　　⌒  
　　　　 　 {{　　 　 ''′    
       ”'  
</pre>
</td>
<td>
<pre>
  <div align="left">
============================================
                 STAG SHELL                 
             - BEETLE COMMANDS -            
============================================
 DATE: 2026-07-18             TIME: 03:03
 TARGET: /dev/rdisk4 (STAG_TEST)
--------------------------------------------
 ENGINE FUNCTIONS LOG EXECUTED:
--------------------------------------------
 [✔] crawl    : File Scraping and Recovery
 [✔] log      : Network and IP Audit and 
                Tracking
 [✔] puncture : Secure Shred Ready
                Target Overwrites Verified
 [✔] pinch    : Disk Triage and Integrity
 [✔] trail    : File Source Tracker
--------------------------------------------
 STATUS       : Work In Progress
============================================
 PROGRESS     : SHA256 Cryptography
============================================
</pre>
</td>
</tr>
</table>
  </div>

## 🪲 Beetle Project REMINDER: Make crawl Footer-Aware

### 1. Current State (Size-Limited)
The current `crawl.c` implementation relies on a fixed guess: once it identifies a magic header, it blindly extracts the next **2,000 sectors** (approx. 1 MB) into a staging file. 
```c
for (int block = 0; block < 2000; block++) {...}
```
* **If the file is smaller than 1 MB:** The recovered file will open, but it will contain "slack space" or trailing data noise from whatever previously occupied the adjacent sectors.
* **If the file is larger than 1 MB:** The recovered file will be cut off too early (truncated), making it corrupt and unopenable.

### 2. Transitioning to Footer-Aware Carving
To make crawl precise, the internal extraction loop must scan every incoming block for specific **End-of-File (EOF) magic bytes** (footers) unique to that file type. 

### 3. Structural Code Architecture Needed
To implement this, update your `FileSignature` struct to support footers:

```c
typedef struct {
    const char *extension;
    const unsigned char *magic_bytes;
    size_t length;
    const unsigned char *footer_bytes;  // <-- Add this
    size_t footer_length;               // <-- Add this
    const char *description;
} FileSignature;
```
This Impementation will be done after all functions' foundational operations are done.