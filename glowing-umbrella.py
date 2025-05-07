import tkinter as tk
from tkinter import ttk, filedialog, messagebox
from ctypes import CDLL, c_char_p, c_int, c_double
import os

# Load compiled C library
tts_lib = CDLL('./tts_lib.dll')  # Windows
# tts_lib = CDLL('./libtts.so')  # Linux

# Define C function prototypes
tts_lib.split_txt_file_gui.argtypes = [c_char_p, c_int]
tts_lib.split_txt_file_gui.restype = None

tts_lib.convertCustom.argtypes = [c_char_p, c_char_p, c_char_p, c_double, c_double]
tts_lib.convertCustom.restype = None

# Voice data from C header (should match voices.c)
VOICES = [
    # English (en_GB)
    ("en_GB: alan (low)", "piper/piper-voices/en/en_GB/alan/low/en_GB-alan-low.onnx"),
    ("en_GB: alan (medium)", "piper/piper-voices/en/en_GB/alan/medium/en_GB-alan-medium.onnx"),
    ("en_GB: alba (medium)", "piper/piper-voices/en/en_GB/alba/medium/en_GB-alba-medium.onnx"),
    ("en_GB: aru (medium)", "piper/piper-voices/en/en_GB/aru/medium/en_GB-aru-medium.onnx"),
    ("en_GB: cori (medium)", "piper/piper-voices/en/en_GB/cori/medium/en_GB-cori-medium.onnx"),
    ("en_GB: cori (high)", "piper/piper-voices/en/en_GB/cori/high/en_GB-cori-high.onnx"),
    ("en_GB: jenny_dioco (medium)", "piper/piper-voices/en/en_GB/jenny_dioco/medium/en_GB-jenny_dioco-medium.onnx"),
    ("en_GB: northern_english_male (medium)", "piper/piper-voices/en/en_GB/northern_english_male/medium/en_GB-northern_english_male-medium.onnx"),
    ("en_GB: semaine (medium)", "piper/piper-voices/en/en_GB/semaine/medium/en_GB-semaine-medium.onnx"),
    ("en_GB: southern_english_female (low)", "piper/piper-voices/en/en_GB/southern_english_female/low/en_GB-southern_english_female-low.onnx"),
    ("en_GB: vctk (medium)", "piper/piper-voices/en/en_GB/vctk/medium/en_GB-vctk-medium.onnx"),

    # English (en_US)
    ("en_US: amy (low)", "piper/piper-voices/en/en_US/amy/low/en_US-amy-low.onnx"),
    ("en_US: amy (medium)", "piper/piper-voices/en/en_US/amy/medium/en_US-amy-medium.onnx"),
    ("en_US: arctic (medium)", "piper/piper-voices/en/en_US/arctic/medium/en_US-arctic-medium.onnx"),
    ("en_US: bryce (medium)", "piper/piper-voices/en/en_US/bryce/medium/en_US-bryce-medium.onnx"),
    ("en_US: danny (low)", "piper/piper-voices/en/en_US/danny/low/en_US-danny-low.onnx"),
    ("en_US: hfc_female (medium)", "piper/piper-voices/en/en_US/hfc_female/medium/en_US-hfc_female-medium.onnx"),
    ("en_US: hfc_male (medium)", "piper/piper-voices/en/en_US/hfc_male/medium/en_US-hfc_male-medium.onnx"),
    ("en_US: joe (medium)", "piper/piper-voices/en/en_US/joe/medium/en_US-joe-medium.onnx"),
    ("en_US: john (medium)", "piper/piper-voices/en/en_US/john/medium/en_US-john-medium.onnx"),
    ("en_US: kathleen (low)", "piper/piper-voices/en/en_US/kathleen/low/en_US-kathleen-low.onnx"),
    ("en_US: kristin (medium)", "piper/piper-voices/en/en_US/kristin/medium/en_US-kristin-medium.onnx"),
    ("en_US: kusal (medium)", "piper/piper-voices/en/en_US/kusal/medium/en_US-kusal-medium.onnx"),
    ("en_US: l2arctic (medium)", "piper/piper-voices/en/en_US/l2arctic/medium/en_US-l2arctic-medium.onnx"),
    ("en_US: lessac (low)", "piper/piper-voices/en/en_US/lessac/low/en_US-lessac-low.onnx"),
    ("en_US: lessac (medium)", "piper/piper-voices/en/en_US/lessac/medium/en_US-lessac-medium.onnx"),
    ("en_US: lessac (high)", "piper/piper-voices/en/en_US/lessac/high/en_US-lessac-high.onnx"),
    ("en_US: libritts (high)", "piper/piper-voices/en/en_US/libritts/high/en_US-libritts-high.onnx"),
    ("en_US: libritts_r (medium)", "piper/piper-voices/en/en_US/libritts_r/medium/en_US-libritts_r-medium.onnx"),
    ("en_US: ljspeech (medium)", "piper/piper-voices/en/en_US/ljspeech/medium/en_US-ljspeech-medium.onnx"),
    ("en_US: ljspeech (high)", "piper/piper-voices/en/en_US/ljspeech/high/en_US-ljspeech-high.onnx"),
    ("en_US: norman (medium)", "piper/piper-voices/en/en_US/norman/medium/en_US-norman-medium.onnx"),
    ("en_US: reza_ibrahim (medium)", "piper/piper-voices/en/en_US/reza_ibrahim/medium/en_US-reza_ibrahim-medium.onnx"),
    ("en_US: ryan (low)", "piper/piper-voices/en/en_US/ryan/low/en_US-ryan-low.onnx"),
    ("en_US: ryan (medium)", "piper/piper-voices/en/en_US/ryan/medium/en_US-ryan-medium.onnx"),
    ("en_US: ryan (high)", "piper/piper-voices/en/en_US/ryan/high/en_US-ryan-high.onnx"),
    ("en_US: sam (medium)", "piper/piper-voices/en/en_US/sam/medium/en_US-sam-medium.onnx"),
    
    # Spanish (es_ES/es_MX)
    ("es_ES: carlfm (x_low)", "piper/piper-voices/es/es_ES/carlfm/x_low/es_ES-carlfm-x_low.onnx"),
    ("es_ES: davefx (medium)", "piper/piper-voices/es/es_ES/davefx/medium/es_ES-davefx-medium.onnx"),
    ("es_ES: mls_10246 (low)", "piper/piper-voices/es/es_ES/mls_10246/low/es_ES-mls_10246-low.onnx"),
    ("es_ES: mls_9972 (low)", "piper/piper-voices/es/es_ES/mls_9972/low/es_ES-mls_9972-low.onnx"),
    ("es_ES: sharvard (medium)", "piper/piper-voices/es/es_ES/sharvard/medium/es_ES-sharvard-medium.onnx"),
    ("es_MX: ald (medium)", "piper/piper-voices/es/es_MX/ald/medium/es_MX-ald-medium.onnx"),
    ("es_MX: claude (high)", "piper/piper-voices/es/es_MX/claude/high/es_MX-claude-high.onnx"),

    # Portuguese (pt_BR/pt_PT)
    ("pt_BR: cadu (medium)", "piper/piper-voices/pt/pt_BR/cadu/medium/pt_BR-cadu-medium.onnx"),
    ("pt_BR: edresson (low)", "piper/piper-voices/pt/pt_BR/edresson/low/pt_BR-edresson-low.onnx"),
    ("pt_BR: faber (medium)", "piper/piper-voices/pt/pt_BR/faber/medium/pt_BR-faber-medium.onnx"),
    ("pt_BR: jeff (medium)", "piper/piper-voices/pt/pt_BR/jeff/medium/pt_BR-jeff-medium.onnx"),
    ("pt_PT: tugao (medium)", "piper/piper-voices/pt/pt_PT/tugao/medium/pt_PT-tugao-medium.onnx"),
]


class PiperTTSGUI:
    def __init__(self, root):
        self.root = root
        root.title("Piper TTS Configuration")
        self.create_widgets()
        self.setup_validation()
        
    def create_widgets(self):
        # Input Folder Section
        ttk.Label(self.root, text="Input Folder:").grid(row=0, column=0, padx=5, pady=5)
        self.input_path = ttk.Entry(self.root, width=50)
        self.input_path.grid(row=0, column=1, padx=5, pady=5)
        ttk.Button(self.root, text="Browse...", command=self.browse_input).grid(row=0, column=2, padx=5)

        # Output Folder Section
        ttk.Label(self.root, text="Output Folder:").grid(row=1, column=0, padx=5, pady=5)
        self.output_path = ttk.Entry(self.root, width=50)
        self.output_path.grid(row=1, column=1, padx=5, pady=5)
        ttk.Button(self.root, text="Browse...", command=self.browse_output).grid(row=1, column=2, padx=5)

        # Voice Selection
        ttk.Label(self.root, text="Voice:").grid(row=2, column=0, padx=5, pady=5)
        self.voice_combo = ttk.Combobox(self.root, values=[v[0] for v in VOICES], state="readonly")
        self.voice_combo.current(1)
        self.voice_combo.grid(row=2, column=1, padx=5, pady=5, columnspan=2, sticky="ew")

        # Speed and Pause Controls
        ttk.Label(self.root, text="Speed Scale (0.001-2.0):").grid(row=3, column=0, padx=5, pady=5)
        self.speed_entry = ttk.Entry(self.root)
        self.speed_entry.insert(0, "1.0")
        self.speed_entry.grid(row=3, column=1, padx=5, pady=5)
        
        ttk.Label(self.root, text="Pause (0.001-2.0):").grid(row=4, column=0, padx=5, pady=5)
        self.pause_entry = ttk.Entry(self.root)
        self.pause_entry.insert(0, "0.2")
        self.pause_entry.grid(row=4, column=1, padx=5, pady=5)

        # Action Buttons
        ttk.Button(self.root, text="Convert", command=self.convert_files).grid(row=5, column=0, pady=10)

        # Split File Section
        ttk.Label(self.root, text="Text File:").grid(row=6, column=0, padx=5, pady=5)
        self.txt_path = ttk.Entry(self.root, width=50)
        self.txt_path.grid(row=6, column=1, padx=5, pady=5)
        ttk.Button(self.root, text="Browse...", command=self.browse_txt).grid(row=6, column=2, padx=5)

        ttk.Label(self.root, text="Words per File:").grid(row=7, column=0, padx=5, pady=5)
        self.words_entry = ttk.Entry(self.root)
        self.words_entry.insert(0, "3700")
        self.words_entry.grid(row=7, column=1, padx=5, pady=5, sticky='w')

        # Action Buttons
        ttk.Button(self.root, text="Split", command=self.split_file).grid(row=8, column=0, pady=10)

        # Action Buttons
        ttk.Button(self.root, text="Generate Samples", command=self.generate_samples).grid(row=9, column=0, pady=10)

    def setup_validation(self):
        self.speed_entry.config(validate="key", validatecommand=(
            self.root.register(self.validate_float), '%P'))
        self.pause_entry.config(validate="key", validatecommand=(
            self.root.register(self.validate_float), '%P'))
        self.words_entry.config(validate="key", validatecommand=(
            self.root.register(self.validate_int), '%P'))

    def validate_float(self, value):
        try:
            if not value: return True
            return 0.001 <= float(value) <= 2.0
        except ValueError:
            return False

    def validate_int(self, value):
        try:
            if not value: return True
            return 1 <= int(value) <= 100000
        except ValueError:
            return False

    def browse_input(self):
        path = filedialog.askdirectory()
        if path:
            self.input_path.delete(0, tk.END)
            self.input_path.insert(0, path)

    def browse_output(self):
        path = filedialog.askdirectory()
        if path:
            self.output_path.delete(0, tk.END)
            self.output_path.insert(0, path)

    def browse_txt(self):
        path = filedialog.askopenfilename(filetypes=[("Text files", "*.txt")])
        if path:
            self.txt_path.delete(0, tk.END)
            self.txt_path.insert(0, path)

    def split_file(self):
        txt_path = self.txt_path.get().encode('utf-8')
        try:
            words = int(self.words_entry.get())
            if not 1 <= words <= 100000:
                raise ValueError
        except ValueError:
            messagebox.showerror("Error", "Enter a valid number between 1-100000")
            return

        tts_lib.split_txt_file_gui(c_char_p(txt_path), c_int(words))
        messagebox.showinfo("Info", "File split completed!")

    def convert_files(self):
        # Validate inputs
        try:
            speed = float(self.speed_entry.get())
            pause = float(self.pause_entry.get())
            if not (0.001 <= speed <= 2.0) or not (0.001 <= pause <= 2.0):
                raise ValueError
        except ValueError:
            messagebox.showerror("Error", "Invalid speed/pause values")
            return

        # Get selected voice
        voice_idx = self.voice_combo.current()
        if voice_idx == -1:
            messagebox.showerror("Error", "Select a voice first")
            return
        model_path = VOICES[voice_idx][1].encode('utf-8')

        # Get paths
        input_path = self.input_path.get().encode('utf-8')
        output_path = self.output_path.get().encode('utf-8')

        # Call C conversion function
        tts_lib.convertCustom(
            c_char_p(model_path),
            c_char_p(input_path),
            c_char_p(output_path),
            c_double(speed),
            c_double(pause)
        )
        messagebox.showinfo("Info", "Conversion completed!")

    def generate_samples(self):
        try:
            # Call C function directly
            tts_lib.generate_voice_samples()
            messagebox.showinfo("Info", "Voice samples generated in output/samples")
        except Exception as e:
            messagebox.showerror("Error", f"Sample generation failed: {str(e)}")

if __name__ == "__main__":
    root = tk.Tk()
    app = PiperTTSGUI(root)
    root.mainloop()
