# Glowing Umbrella - Piper TTS Integration

A cross-platform text-to-speech system using Piper TTS with C integration.

## Prerequisites

### Windows (MSYS2/MinGW)
1. **Install MSYS2**  
   Download from [msys2.org](https://www.msys2.org/)  
   Run installer and update packages:
pacman -Syu

2. **Install MinGW64 Toolchain**  
In MSYS2 terminal:
pacman -S mingw-w64-x86_64-toolchain

3. **Install Development Libraries**  
pacman -S mingw-w64-x86_64-libsndfile mingw-w64-x86_64-opus

### Linux
sudo apt install build-essential libsndfile1-dev

---

## Setup

1. **Clone Repository**
git clone https://github.com/jaredsbror/glowing-umbrella.git
cd glowing-umbrella

2. **Download Piper Voices**  
Place voice models in `piper/piper-voices/` directory.

3. **Build Project**  
make # Or use your preferred build system

---

## Running

### Windows (MSYS2)
export MSYS_NO_PATHCONV=1
./your_program.exe


### Linux
./your_program

---

## Python Setup (Optional)
python -m venv venv
source venv/bin/activate
pip install -r requirements.txt


---

## Troubleshooting

### "File Not Found" Errors
- Use absolute paths in commands
- Verify working directory with `pwd`
- Check file permissions

### Piper Hanging on Input
1. Test with small input file
2. Check file encoding (UTF-8 without BOM)
3. Update Piper executable from [releases](https://github.com/rhasspy/piper/releases)

### Cross-Platform Path Handling
#ifdef _WIN32
#define PATH_SEP '\'
#else
#define PATH_SEP '/'
#endif


---

## Project Structure
glowing-umbrella/
├── piper_win/ # Piper Windows binaries
├── piper/ # Voice models
├── input/ # Text files for processing
├── output/ # Generated audio files
├── src/ # C source files
└── Makefile

---

## Key Features
- Cross-platform directory creation
- Case-insensitive voice selection
- Batch file processing
- Real-time audio parameter adjustment

[View full documentation](https://github.com/jaredsbror/glowing-umbrella/wiki)
