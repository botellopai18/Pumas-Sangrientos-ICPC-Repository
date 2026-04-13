import os
import re
import subprocess

# Configuración de rutas
CODE_DIR = 'code'
TEMPLATE_FILE = 'template.tex'
OUTPUT_FILE = 'notebook.tex'

# Patrones de "Boilerplate" que queremos eliminar de los códigos específicos
BOILERPLATE_PATTERNS = [
    r'#include\s*<.*>',            # Librerías
    r'using\s+namespace\s+std;',    # Namespace
    r'typedef\s+.*;',               # Typedefs
    r'#define\s+.*',                # Macros
    r'void\s+fast_io\s*\([^)]*\)\s*\{[^\}]*\}', # Función fast_io completa
    r'const\s+ll\s+m\s*=\s*.*;',    # Opcional: constantes comunes como el MOD
    r'ios_base::sync_with_stdio.*;', # Líneas de fast_io sueltas
    r'cin\.tie.*;',
    r'cout\.tie.*;'
]

def clean_code_content(content, is_template=False):
    """Elimina metadatos y código repetitivo (boilerplate)."""
    # 1. Eliminar el bloque de comentarios de metadatos /** ... */
    content = re.sub(r'/\*\*.*?\*/', '', content, flags=re.DOTALL)
    
    if is_template:
        return content.strip()

    # 2. Eliminar líneas de boilerplate definidas arriba
    for pattern in BOILERPLATE_PATTERNS:
        content = re.sub(pattern, '', content, flags=re.IGNORECASE)

    # 3. Eliminar la función main() estándar si está vacía o solo tiene solve()
    # Este regex busca un main que solo llame a fast_io, solve o tenga return 0
    main_pattern = r'int\s+main\s*\(\s*\)\s*\{[^\}]*\}'
    content = re.sub(main_pattern, '', content, flags=re.DOTALL)

    # Limpieza final de saltos de línea excesivos
    content = re.sub(r'\n\s*\n', '\n', content)
    return content.strip()

def parse_metadata(content):
    """Extrae Título, Descripción y Autor de los comentarios."""
    title, description, author = None, "Sin descripción.", "Desconocido"
    match = re.search(r'/\*\*(.*?)\*/', content, re.DOTALL)
    if match:
        block = match.group(1)
        t_match = re.search(r'\*\s*Title:\s*(.*)', block, re.IGNORECASE)
        if t_match: title = t_match.group(1).strip()
        a_match = re.search(r'\*\s*Author:\s*(.*)', block, re.IGNORECASE)
        if a_match: author = a_match.group(1).strip()
        d_match = re.search(r'\*\s*Description:\s*(.*)', block, re.IGNORECASE)
        if d_match: description = d_match.group(1).strip()
    return title, description, author

def compile_and_clean():
    """Compila y limpia archivos temporales."""
    print("Compilando PDF...")
    subprocess.run(['pdflatex', '-interaction=nonstopmode', OUTPUT_FILE], stdout=subprocess.DEVNULL)
    subprocess.run(['pdflatex', '-interaction=nonstopmode', OUTPUT_FILE], stdout=subprocess.DEVNULL)
    
    base = os.path.splitext(OUTPUT_FILE)[0]
    for ext in ['.aux', '.log', '.toc', '.out', '.tex']:
        if os.path.exists(base + ext): os.remove(base + ext)
    print("¡Proceso completado!")

def generate_notebook():
    if not os.path.exists(CODE_DIR): return
    latex_content = ""
    
    directories = sorted([d for d in os.listdir(CODE_DIR) if os.path.isdir(os.path.join(CODE_DIR, d))])
    
    for folder in directories:
        latex_content += f"\\section{{{folder.replace('_', ' ').title()}}}\n\n"
        folder_path = os.path.join(CODE_DIR, folder)
        files = sorted([f for f in os.listdir(folder_path) if f.endswith(('.cpp', '.py', '.java'))])
        
        for file in files:
            file_path = os.path.join(folder_path, file)
            with open(file_path, 'r', encoding='utf-8') as f:
                raw_content = f.read()
            
            # Detectar si es el archivo de plantilla principal
            is_main_template = "template" in file.lower()
            
            # Obtener metadatos y limpiar el código
            m_title, desc, author = parse_metadata(raw_content)
            clean_code = clean_code_content(raw_content, is_template=is_main_template)
            
            title = m_title if m_title else file.replace('_', ' ').replace('.cpp', '').title()
            
            # Escribir el código limpio a un archivo temporal para lstinputlisting
            # o usar el comando directo de listings
            latex_content += f"\\subsection{{{title}}}\n"
            latex_content += f"\\textbf{{Autor:}} {author} \\\\\n"
            latex_content += f"\\textbf{{Descripción:}} {desc}\n\n"
            latex_content += f"\\begin{{lstlisting}}\n{clean_code}\n\\end{{lstlisting}}\n\n"

    with open(TEMPLATE_FILE, 'r', encoding='utf-8') as f:
        template = f.read()
    
    with open(OUTPUT_FILE, 'w', encoding='utf-8') as f:
        f.write(template.replace('% == GENERATOR_CONTENT ==', latex_content))
    
    compile_and_clean()

if __name__ == '__main__':
    generate_notebook()