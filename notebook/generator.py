import os
import re
import subprocess

# Configuración de rutas
CODE_DIR = 'code'
TEMPLATE_FILE = 'template.tex'
OUTPUT_FILE = 'notebook.tex'

BOILERPLATE_PATTERNS = [
    r'#include\s*<.*>',
    r'using\s+namespace\s+std;',
    r'typedef\s+.*;',
    r'#define\s+.*',
    r'void\s+fast_io\s*\([^)]*\)\s*\{[^\}]*\}',
    r'const\s+ll\s+m\s*=\s*.*;',
    r'ios_base::sync_with_stdio.*;',
    r'cin\.tie.*;',
    r'cout\.tie.*;'
]

def clean_code_content(content, is_template=False):
    # Eliminar cualquier bloque de comentario /* ... */ o /** ... */ al inicio
    content = re.sub(r'/\*.*?\*/', '', content, flags=re.DOTALL)
    if is_template: return content.strip()
    for pattern in BOILERPLATE_PATTERNS:
        content = re.sub(pattern, '', content, flags=re.IGNORECASE)
    main_pattern = r'int\s+main\s*\(\s*\)\s*\{[^\}]*\}'
    content = re.sub(main_pattern, '', content, flags=re.DOTALL)
    content = re.sub(r'\n\s*\n', '\n', content)
    return content.strip()

def parse_metadata_and_comment(content):
    """Extrae el título y limpia el bloque completo de comentarios para usarlo como texto."""
    title = None
    full_comment = ""
    
    # Captura tanto /* como /**
    match = re.search(r'/\*(.*?)\*/', content, re.DOTALL)
    if match:
        block = match.group(1)
        
        # Buscar el título específicamente para la sección de LaTeX
        t_match = re.search(r'\*\s*Title:\s*(.*)', block, re.IGNORECASE)
        if t_match: 
            title = t_match.group(1).strip()
        
        # Limpiar línea por línea quitando los asteriscos decorativos del inicio
        cleaned_lines = []
        for line in block.split('\n'):
            line_str = line.strip()
            if line_str.startswith('*'):
                line_str = line_str[1:].strip()
            cleaned_lines.append(line_str)
            
        full_comment = "\n".join(cleaned_lines).strip()
        
    return title, full_comment

def format_comment_to_latex(text):
    """Detecta bloques matemáticos $...$ para mantenerlos intactos y escapa el texto normal ordenadamente."""
    # Dividimos el texto usando el delimitador de matemáticas $
    parts = text.split('$')
    latex_parts = []
    
    # ¡CRÍTICO! El reemplazo de '\\' va al inicio para asegurar las barras originales
    replacements = [
        ('\\', r'\textsl{\textbackslash{}}'), # Muestra la barra original de forma elegante
        ('&', r'\&'),
        ('%', r'\%'),
        ('#', r'\#'),
        ('_', r'\_'),
        ('{', r'\{'),
        ('}', r'\}'),
        ('~', r'\textasciitilde{}'),
        ('^', r'\textasciicircum{}'),
        ('<', r'\textless{}'),
        ('>', r'\textgreater{}')
    ]
    
    for idx, part in enumerate(parts):
        if idx % 2 == 1:
            # Índice impar significa que estaba CERRADO entre dos signos $
            # Conservamos la fórmula matemática original intacta
            latex_parts.append('$' + part + '$')
        else:
            # Índice par es texto o comentarios normales, aplicamos la cascada ordenada
            line = part
            for key, val in replacements:
                line = line.replace(key, val)
            latex_parts.append(line)
            
    # Unimos las partes procesadas
    full_text = "".join(latex_parts)
    
    # Formateamos los saltos de línea con párrafos estables sin usar dobles diagonales destructivas
    lines = full_text.split('\n')
    final_lines = []
    for line in lines:
        if line.strip() == "":
            final_lines.append(r'\par\smallskip')
        else:
            final_lines.append(r'\par\noindent ' + line)
            
    return '\n'.join(final_lines)


def process_file(file_path, file_name, level):
    """Genera el bloque LaTeX para un archivo individual con su documentación."""
    with open(file_path, 'r', encoding='utf-8') as f:
        raw_content = f.read()
    
    is_main_template = "template" in file_name.lower()
    title, full_comment = parse_metadata_and_comment(raw_content)
    clean_code = clean_code_content(raw_content, is_template=is_main_template)
    
    if not title:
        title = file_name.replace('_', ' ').split('.')[0].title()
    
    section_cmd = "subsection" if level == 2 else "subsubsection"
    block = f"\\{section_cmd}{{{title}}}\n"
    
    # Si el archivo tiene un bloque de comentarios, lo inyectamos elegantemente como texto
    if full_comment:
        formatted_comment = format_comment_to_latex(full_comment)
        block += f"\\begin{{flushleft}}\n"
        block += f"\\footnotesize\\ttfamily\n"
        block += f"{formatted_comment}\n"
        block += f"\\end{{flushleft}}\n\n"
    
    block += f"\\begin{{lstlisting}}\n{clean_code}\n\\end{{lstlisting}}\n\n"
    return block

def generate_notebook():
    if not os.path.exists(CODE_DIR): return
    latex_content = ""
    
    top_dirs = sorted([d for d in os.listdir(CODE_DIR) if os.path.isdir(os.path.join(CODE_DIR, d))])
    
    for folder in top_dirs:
        top_path = os.path.join(CODE_DIR, folder)
        latex_content += f"\\section{{{folder.replace('_', ' ').title()}}}\n\n"
        
        items = sorted(os.listdir(top_path))
        
        # 1. Procesar Subcarpetas (Nivel 2)
        for item in items:
            item_path = os.path.join(top_path, item)
            if os.path.isdir(item_path):
                latex_content += f"\\subsection{{{item.replace('_', ' ').title()}}}\n\n"
                sub_files = sorted([f for f in os.listdir(item_path) if f.endswith(('.cpp', '.py', '.java'))])
                for f in sub_files:
                    latex_content += process_file(os.path.join(item_path, f), f, level=3)
        
        # 2. Procesar Archivos sueltos en la raíz de la sección
        for item in items:
            item_path = os.path.join(top_path, item)
            if os.path.isfile(item_path) and item.endswith(('.cpp', '.py', '.java')):
                latex_content += process_file(item_path, item, level=2)

    with open(TEMPLATE_FILE, 'r', encoding='utf-8') as f:
        template = f.read()
    with open(OUTPUT_FILE, 'w', encoding='utf-8') as f:
        f.write(template.replace('% == GENERATOR_CONTENT ==', latex_content))
    
    compile_and_clean()

def compile_and_clean():
    print("Compilando PDF (Paso 1/3)...")
    # Quitamos el DEVNULL temporalmente para que si truena, veas el error real en la terminal
    subprocess.run(['pdflatex', '-interaction=nonstopmode', OUTPUT_FILE])
    
    print("Compilando PDF (Paso 2/3)...")
    subprocess.run(['pdflatex', '-interaction=nonstopmode', OUTPUT_FILE], stdout=subprocess.DEVNULL)
    
    print("Compilando PDF (Paso 3/3 - Estabilizando Indice)...")
    subprocess.run(['pdflatex', '-interaction=nonstopmode', OUTPUT_FILE], stdout=subprocess.DEVNULL)
    
    base = os.path.splitext(OUTPUT_FILE)[0]
    # CONSEJO DE ICPC: No borres el .toc inmediatamente si vas a estar editando seguido, 
    # pero si lo dejas, al menos ya se habrá renderizado en el tercer paso.
    for ext in ['.aux', '.log', '.out']:
        if os.path.exists(base + ext): 
            os.remove(base + ext)
    print("¡Proceso completado exitosamente! Archivo: notebook.pdf")

if __name__ == '__main__':
    generate_notebook()