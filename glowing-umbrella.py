import tkinter as tk
from tkinter import ttk, filedialog

# Voice data extracted from your C header
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
    ("en_US: ryan (low)", "piper/piper-voices/en/en_US/ryan/low/en_US-ryan-low.onnx"),
    ("en_US: ryan (medium)", "piper/piper-voices/en/en_US/ryan/medium/en_US-ryan-medium.onnx"),
    ("en_US: ryan (high)", "piper/piper-voices/en/en_US/ryan/high/en_US-ryan-high.onnx"),
]

class PiperGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Piper TTS Configuration")
        
        # Input Folder
        ttk.Label(root, text="Input Folder:").grid(row=0, column=0, padx=5, pady=5, sticky="w")
        self.input_folder = tk.StringVar()
        ttk.Entry(root, textvariable=self.input_folder, width=40).grid(row=0, column=1, padx=5)
        ttk.Button(root, text="Browse...", command=self.select_input_folder).grid(row=0, column=2, padx=5)

        # Output Folder
        ttk.Label(root, text="Output Folder:").grid(row=1, column=0, padx=5, pady=5, sticky="w")
        self.output_folder = tk.StringVar()
        ttk.Entry(root, textvariable=self.output_folder, width=40).grid(row=1, column=1, padx=5)
        ttk.Button(root, text="Browse...", command=self.select_output_folder).grid(row=1, column=2, padx=5)

        # Speed Scale
        ttk.Label(root, text="Speed Scale (0.001-2.0):").grid(row=2, column=0, padx=5, pady=5, sticky="w")
        self.speed_var = tk.DoubleVar(value=1.0)
        self.speed_entry = ttk.Entry(root, textvariable=self.speed_var, validate="key")
        self.speed_entry.configure(validatecommand=(
            self.speed_entry.register(self.validate_float_range), '%P', 0.001, 2.0))
        self.speed_entry.grid(row=2, column=1, padx=5, pady=5, sticky="w")

        # Sentence Pause
        ttk.Label(root, text="Sentence Pause (0.001-2.0):").grid(row=3, column=0, padx=5, pady=5, sticky="w")
        self.pause_var = tk.DoubleVar(value=0.2)
        self.pause_entry = ttk.Entry(root, textvariable=self.pause_var, validate="key")
        self.pause_entry.configure(validatecommand=(
            self.pause_entry.register(self.validate_float_range), '%P', 0.001, 2.0))
        self.pause_entry.grid(row=3, column=1, padx=5, pady=5, sticky="w")

        # Voice Selection
        ttk.Label(root, text="Voice:").grid(row=4, column=0, padx=5, pady=5, sticky="w")
        self.voice_var = tk.StringVar()
        self.voice_combo = ttk.Combobox(root, textvariable=self.voice_var, 
                                      values=[v[0] for v in VOICES], state="readonly")
        self.voice_combo.current(0)
        self.voice_combo.grid(row=4, column=1, padx=5, pady=5, sticky="ew")

    def validate_float_range(self, new_value, min_val, max_val):
        try:
            if not new_value:
                return True
            value = float(new_value)
            return float(min_val) <= value <= float(max_val)
        except ValueError:
            return False

    def select_input_folder(self):
        folder = filedialog.askdirectory(title="Select Input Folder")
        if folder:
            self.input_folder.set(folder)

    def select_output_folder(self):
        folder = filedialog.askdirectory(title="Select Output Folder")
        if folder:
            self.output_folder.set(folder)

if __name__ == "__main__":
    root = tk.Tk()
    app = PiperGUI(root)
    root.mainloop()
