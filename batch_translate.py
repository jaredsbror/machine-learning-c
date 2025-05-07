import tkinter as tk
from tkinter import filedialog, messagebox
import os
import argostranslate.translate

def translate_files():
    src_lang = src_entry.get().strip()
    dest_lang = dest_entry.get().strip()

    if not src_lang or not dest_lang:
        messagebox.showerror("Error", "Please enter both language codes")
        return

    files = listbox.get(0, tk.END)
    if not files:
        messagebox.showerror("Error", "No files selected")
        return

    for file_path in files:
        filename = os.path.basename(file_path)  # Get filename early for error handling
        try:
            # Try multiple encodings
            encodings = ['utf-8', 'latin-1', 'cp1252', 'utf-16']
            original_text = None

            for encoding in encodings:
                try:
                    with open(file_path, 'r', encoding=encoding) as f:
                        original_text = f.read()
                    break
                except UnicodeDecodeError:
                    continue

            if original_text is None:
                raise UnicodeDecodeError(f"Failed to decode {filename} with tried encodings: {encodings}")

            # Translate
            translated = argostranslate.translate.translate(original_text, src_lang, dest_lang)

            # Create new filename
            dir_path, filename = os.path.split(file_path)
            name, ext = os.path.splitext(filename)
            new_name = f"{name}_{src_lang}_to_{dest_lang}{ext}"
            new_path = os.path.join(dir_path, new_name)

            # Write with UTF-8
            with open(new_path, 'w', encoding='utf-8') as f:
                f.write(translated)

        except Exception as e:
            messagebox.showerror("Translation Error", f"Failed to translate {filename}:\n{str(e)}")
            return

    messagebox.showinfo("Success", f"Translated {len(files)} files")

def add_files():
    files = filedialog.askopenfilenames(filetypes=[("Text files", "*.txt")])
    if files:
        listbox.delete(0, tk.END)
        for f in files:
            listbox.insert(tk.END, f)

# Create GUI
root = tk.Tk()
root.title("Batch File Translator")
root.geometry("500x400")

# Language inputs
tk.Label(root, text="Source Language Code (e.g., en):").pack(pady=5)
src_entry = tk.Entry(root)
src_entry.pack()

tk.Label(root, text="Destination Language Code (e.g., pt):").pack(pady=5)
dest_entry = tk.Entry(root)
dest_entry.pack()

# File selection
tk.Button(root, text="Select TXT Files", command=add_files).pack(pady=10)
listbox = tk.Listbox(root, width=60, height=10)
listbox.pack(pady=5, padx=10, fill=tk.BOTH, expand=True)

# Translate button
tk.Button(root, text="Translate Files", command=translate_files, bg="#4CAF50", fg="white").pack(pady=10)

root.mainloop()
