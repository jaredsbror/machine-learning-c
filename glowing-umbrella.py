import tkinter as tk
from tkinter import ttk, filedialog, messagebox
import os, subprocess

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


WORDS_TO_REPLACE = [
    ("the hell", "the heck"),
    ("the fuck", "the heck"),
    # Add more pairs as needed
]

def get_project_root(self):
        path = os.path.abspath(os.getcwd())
        while True:
            if os.path.basename(path) == "glowing-umbrella":
                return path
            parent = os.path.dirname(path)
            if parent == path:
                raise RuntimeError("Could not find project root")
            path = parent

class PiperTTSGUI:
    def __init__(self, root):
        self.root = root
        root.title("Glowing Umbrella - Piper TTS Configuration")
        self.create_widgets()
        self.setup_validation()

    def create_widgets(self):
        # Input/Output
        ttk.Label(self.root, text="Input Folder:").grid(row=0, column=0, padx=5, pady=5)
        self.input_path = ttk.Entry(self.root, width=50)
        self.input_path.grid(row=0, column=1, padx=5, pady=5)
        ttk.Button(self.root, text="Browse...", command=self.browse_input).grid(row=0, column=2, padx=5)

        ttk.Label(self.root, text="Output Folder:").grid(row=1, column=0, padx=5, pady=5)
        self.output_path = ttk.Entry(self.root, width=50)
        self.output_path.grid(row=1, column=1, padx=5, pady=5)
        ttk.Button(self.root, text="Browse...", command=self.browse_output).grid(row=1, column=2, padx=5)

        # Voice
        ttk.Label(self.root, text="Voice:").grid(row=2, column=0, padx=5, pady=5)
        self.voice_combo = ttk.Combobox(
            self.root,
            values=[v[0] for v in VOICES],  # Combine lang tag and name
            state="readonly"
        )
        self.voice_combo.current(0)
        self.voice_combo.grid(row=2, column=1, padx=5, pady=5, columnspan=2, sticky="ew")


        # Speed/Pause
        ttk.Label(self.root, text="Speed Scale (0.001-2.0):").grid(row=3, column=0, padx=5, pady=5)
        self.speed_entry = ttk.Entry(self.root)
        self.speed_entry.insert(0, "1.0")
        self.speed_entry.grid(row=3, column=1, padx=5, pady=5)

        ttk.Label(self.root, text="Pause (0.001-2.0):").grid(row=4, column=0, padx=5, pady=5)
        self.pause_entry = ttk.Entry(self.root)
        self.pause_entry.insert(0, "0.2")
        self.pause_entry.grid(row=4, column=1, padx=5, pady=5)

        # Convert
        ttk.Button(self.root, text="Convert", command=self.convert_files).grid(row=5, column=0, pady=10)

        # Split File
        ttk.Label(self.root, text="Text File:").grid(row=6, column=0, padx=5, pady=5)
        self.txt_path = ttk.Entry(self.root, width=50)
        self.txt_path.grid(row=6, column=1, padx=5, pady=5)
        ttk.Button(self.root, text="Browse...", command=self.browse_txt).grid(row=6, column=2, padx=5)

        ttk.Label(self.root, text="Words per File:").grid(row=7, column=0, padx=5, pady=5)
        self.words_entry = ttk.Entry(self.root)
        self.words_entry.insert(0, "3700")
        self.words_entry.grid(row=7, column=1, padx=5, pady=5, sticky='w')

        ttk.Button(self.root, text="Split", command=self.split_file).grid(row=8, column=0, pady=10)
        ttk.Button(self.root, text="Replace Words", command=self.replace_phrases_in_file).grid(row=8, column=1, pady=10)
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


    def convert_files(self):
        try:
            # Get selected voice/model
            model_idx = self.voice_combo.current()
            if model_idx == -1:
                messagebox.showerror("Error", "Select a voice first.")
                return
            model_relpath = VOICES[model_idx][1]

            input_dir = self.input_path.get()
            output_dir = self.output_path.get()
            try:
                length_scale = float(self.speed_entry.get())
                sentence_silence = float(self.pause_entry.get())
                if not (0.001 <= length_scale <= 2.0) or not (0.001 <= sentence_silence <= 2.0):
                    raise ValueError
            except ValueError:
                messagebox.showerror("Error", "Invalid speed or pause value.")
                return

            # Find project root
            project_root = get_project_root(self)

            # Build absolute model path
            model_path = os.path.join(project_root, model_relpath)
            if not os.path.isfile(model_path):
                messagebox.showerror("Error", f"Model file not found:\n{model_path}")
                return

            # Ensure output directory exists
            os.makedirs(output_dir, exist_ok=True)

            # Process each .txt file in input_dir
            files_converted = 0
            for filename in os.listdir(input_dir):
                if not filename.lower().endswith(".txt"):
                    continue
                base = os.path.splitext(filename)[0]
                input_path = os.path.join(input_dir, filename)
                output_path = os.path.join(output_dir, f"{base}.wav")
                command = (
                    f"bash -c 'MSYS2_ARG_CONV_EXCL=\"*\" cat \"{input_path}\" | "
                    f"\"{project_root}/piper_win/piper.exe\" "
                    f"--model \"{model_path}\" --length_scale {length_scale:.2f} --sentence_silence {sentence_silence:.2f} "
                    f"--output_file \"{output_path}\""
                )
                print(f"Processing: {filename}")
                print("COMMAND:", command)
                result = subprocess.run(command, shell=True, capture_output=True, text=True)
                print("STDOUT:", result.stdout)
                print("STDERR:", result.stderr)
                if result.returncode == 0:
                    files_converted += 1
                else:
                    print(f"Failed: {filename}")

            messagebox.showinfo("Done", f"Converted {files_converted} files to audio.")
        except Exception as e:
            messagebox.showerror("Error", f"Conversion failed: {e}")

    def split_file(self):
        txt_path = self.txt_path.get()
        try:
            words_per_file = int(self.words_entry.get())
            if not 1 <= words_per_file <= 100000:
                raise ValueError
        except ValueError:
            messagebox.showerror("Error", "Enter a valid number between 1 and 100000 for words per file.")
            return

        if not os.path.isfile(txt_path):
            messagebox.showerror("Error", "Selected text file does not exist.")
            return

        try:
            with open(txt_path, "r", encoding="utf-8") as f:
                words = f.read().split()
        except Exception as e:
            messagebox.showerror("Error", f"Could not read file:\n{e}")
            return

        dir_name = os.path.dirname(txt_path)
        base_name = os.path.basename(txt_path)
        base, ext = os.path.splitext(base_name)
        if ext.lower() == ".txt":
            ext = ".txt"
        else:
            ext = ""

        file_idx = 100
        total_words = len(words)
        files_created = 0
        try:
            for i in range(0, total_words, words_per_file):
                chunk_words = words[i:i+words_per_file]
                out_filename = f"{base}__{file_idx}{ext}"
                out_path = os.path.join(dir_name, out_filename)
                with open(out_path, "w", encoding="utf-8") as out_file:
                    out_file.write(" ".join(chunk_words))
                file_idx += 1
                files_created += 1
        except Exception as e:
            messagebox.showerror("Error", f"Could not write split file:\n{e}")
            return

        messagebox.showinfo("Success", f"File split into {files_created} files.")


    def replace_phrases_in_file(self):
        txt_path = self.txt_path.get()
        if not os.path.isfile(txt_path):
            messagebox.showerror("Error", "Selected text file does not exist.")
            return

        try:
            with open(txt_path, "r", encoding="utf-8") as f:
                text = f.read()
            # Replace all phrases
            for old, new in WORDS_TO_REPLACE:
                text = text.replace(old, new)
            # Build output path
            base, ext = os.path.splitext(txt_path)
            if ext.lower() == ".txt":
                out_path = f"{base}_quick_replace.txt"
            else:
                out_path = f"{txt_path}_quick_replace.txt"
            with open(out_path, "w", encoding="utf-8") as f:
                f.write(text)
            messagebox.showinfo("Success", f"File saved as:\n{out_path}")
        except Exception as e:
            messagebox.showerror("Error", f"Error processing file:\n{e}")


    def generate_samples(self):
        try:
            project_root = get_project_root(self)
            input_path = os.path.join(project_root, "input", "DO NOT DELETE", "DO NOT DELETE.txt")
            output_dir = os.path.join(project_root, "output", "samples")
            os.makedirs(output_dir, exist_ok=True)
            length_scales = [0.6, 0.7, 0.8, 0.9, 1.00]
            sentence_silences = [0.05, 0.1, 0.15, 0.2, 0.25, 0.30]

            for display_name, model_relpath in VOICES:
                # Only generate for English voices as in C code
                if not display_name.startswith("en_"):
                    continue
                model_path = os.path.join(project_root, model_relpath)
                base_model = os.path.splitext(os.path.basename(model_path))[0]
                for ls in length_scales:
                    for ss in sentence_silences:
                        output_file = f"{base_model}_ls{ls:.2f}_ss{ss:.2f}.wav"
                        output_path = os.path.join(output_dir, output_file)
                        command = (
                            f"bash -c 'MSYS2_ARG_CONV_EXCL=\"*\" cat \"{input_path}\" | "
                            f"\"{project_root}/piper_win/piper.exe\" "
                            f"--model \"{model_path}\" --length_scale {ls:.2f} --sentence_silence {ss:.2f} "
                            f"--output_file \"{output_path}\""
                        )
                        print(f"Executing: {command}")
                        result = subprocess.run(command, shell=True)
                        if result.returncode != 0:
                            print(f"ERROR generating {output_path}")

            messagebox.showinfo("Done", "Voice samples generated in output/samples.")
        except Exception as e:
            messagebox.showerror("Error", f"Sample generation failed: {e}")


if __name__ == "__main__":
    root = tk.Tk()
    app = PiperTTSGUI(root)
    root.mainloop()
