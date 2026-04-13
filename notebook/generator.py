import os
import re

CODE_DIR = "./code"
TEMPLATE_FILE = "template.tex"
OUTPUT_FILE = "notebook.tex"

def get_metadata(filepath):
    """Extrae la descripción y el autor de los comentarios del .cpp"""
    desc = "Sin descripción."
    author = "Equipo"
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            content = f.read()
            # Buscamos usando expresiones regulares
            d_match = re.search(r"Description:\s*(.*)", content)
            a_match = re.search(r"Author:\s*(.*)", content)
            if d_match: desc = d_match.group(1).strip()
            if a_match: author = a_match.group(1).strip()
    except:
        pass
    return desc, author

def generate_tex():
    if not os.path.exists(TEMPLATE_FILE):
        print("Error: No existe template.tex")
        return

    with open(TEMPLATE_FILE, 'r', encoding='utf-8') as f:
        template_lines = f.readlines()

    with open(OUTPUT_FILE, 'w', encoding='utf-8') as f:
        for line in template_lines:
            if "% --- SECCIÓN DE EJEMPLO ---" in line:
                # Escanear carpetas
                for root, dirs, files in os.walk(CODE_DIR):
                    cpp_files = [f for f in files if f.endswith('.cpp')]
                    if not cpp_files: continue
                    
                    section_name = os.path.basename(root).replace("_", " ").title()
                    f.write(f"\\section{{{section_name}}}\n")
                    
                    for cpp_file in sorted(cpp_files):
                        path = os.path.join(root, cpp_file).replace("\\", "/")
                        algo_name = cpp_file.replace(".cpp", "").replace("_", " ").title()
                        
                        # Obtener metadatos
                        desc, author = get_metadata(path)
                        
                        # Escribir en LaTeX con formato bonito
                        f.write(f"\\subsection{{{algo_name}}}\n")
                        f.write(f"\\textbf{{Autor:}} {author} \\\\ \n")
                        f.write(f"\\textit{{ {desc} }} \\\\ \n")
                        f.write(f"\\lstinputlisting{{{path}}}\n")
                        f.write(f"\\vspace{{0.5cm}}\n")
                continue
            f.write(line)

    print(f"✅ Notebook generado con autores y descripciones.")

if __name__ == "__main__":
    generate_tex()