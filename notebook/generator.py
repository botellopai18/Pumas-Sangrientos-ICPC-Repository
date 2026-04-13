import os

code_dir = "./code"
output_tex = "notebook.tex"

with open("template.tex", "r") as f:
    template = f.read()

content = ""
for root, dirs, files in os.walk(code_dir):
    if not files: continue
    
    section_name = os.path.basename(root).replace("_", " ").capitalize()
    content += f"\\section{{{section_name}}}\n"
    
    for file in sorted(files):
        if file.endswith(".cpp"):
            item_name = file.replace(".cpp", "").replace("_", " ")
            file_path = os.path.join(root, file).replace("\\", "/")
            content += f"\\subsection{{{item_name}}}\n"
            content += f"\\lstinputlisting{{{file_path}}}\n"

final_tex = template.replace("% AQUÍ EL SCRIPT INSERTARÁ EL CÓDIGO", content)

with open(output_tex, "w") as f:
    f.write(final_tex)

print("¡Notebook generado! Ahora corre: pdflatex notebook.tex")
