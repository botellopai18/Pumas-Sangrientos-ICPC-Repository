import os
import re

# Configuración de rutas
CODE_DIR = 'code'
TEMPLATE_FILE = 'template.tex'
OUTPUT_FILE = 'notebook.tex'

def parse_metadata(filepath):
    """Extrae la Descripción y el Autor de los comentarios del código."""
    description = "Sin descripción."
    author = "Desconocido"
    
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            content = f.read()
            
            # Busca el primer bloque de comentarios /** ... */
            match = re.search(r'/\*\*(.*?)\*/', content, re.DOTALL)
            if match:
                block = match.group(1)
                
                # Extrae el Author
                author_match = re.search(r'\*\s*Author:\s*(.*)', block, re.IGNORECASE)
                if author_match:
                    author = author_match.group(1).strip()
                
                # Extrae el Description
                desc_match = re.search(r'\*\s*Description:\s*(.*)', block, re.IGNORECASE)
                if desc_match:
                    description = desc_match.group(1).strip()
                    
    except Exception as e:
        print(f"Error procesando el archivo {filepath}: {e}")
        
    return description, author

def clean_title(filename):
    """Limpia el nombre del archivo para usarlo como título en el índice."""
    name = os.path.splitext(filename)[0]
    # Reemplaza guiones bajos por espacios y capitaliza
    return name.replace('_', ' ').title()

def generate_notebook():
    if not os.path.exists(CODE_DIR):
        print(f"Error: No se encontró el directorio '{CODE_DIR}'.")
        return

    latex_content = ""
    
    # 1. Obtener y ordenar las carpetas (Secciones)
    directories = sorted([d for d in os.listdir(CODE_DIR) if os.path.isdir(os.path.join(CODE_DIR, d))])
    
    for folder in directories:
        # Crea la sección en LaTeX
        section_name = clean_title(folder)
        latex_content += f"\\section{{{section_name}}}\n\n"
        
        folder_path = os.path.join(CODE_DIR, folder)
        # 2. Obtener y ordenar los archivos (Subsecciones)
        files = sorted([f for f in os.listdir(folder_path) if f.endswith(('.cpp', '.py', '.java'))])
        
        for file in files:
            file_path = os.path.join(folder_path, file).replace('\\', '/') # Ajuste para rutas en Windows/Linux
            title = clean_title(file)
            
            # Extraer metadata
            desc, author = parse_metadata(file_path)
            
            # 3. Crear la estructura en LaTeX
            latex_content += f"\\subsection{{{title}}}\n"
            latex_content += f"\\textbf{{Autor:}} {author} \\\\\n"
            latex_content += f"\\textbf{{Descripción:}} {desc}\n\n" # La descripción puede contener $$ nativos de LaTeX
            latex_content += f"\\vspace{{0.2cm}}\n"
            latex_content += f"\\lstinputlisting{{{file_path}}}\n\n"

    # 4. Leer la plantilla y fusionar
    try:
        with open(TEMPLATE_FILE, 'r', encoding='utf-8') as f:
            template = f.read()
    except Exception as e:
        print(f"Error leyendo la plantilla {TEMPLATE_FILE}: {e}")
        return

    # Inyectar el código generado en la marca especial
    final_latex = template.replace('% == GENERATOR_CONTENT ==', latex_content)
    
    # 5. Guardar el archivo final
    with open(OUTPUT_FILE, 'w', encoding='utf-8') as f:
        f.write(final_latex)
        
    print(f"¡Éxito! Archivo generado: {OUTPUT_FILE}")
    print("Ahora puedes compilar tu PDF ejecutando: pdflatex notebook.tex")

if __name__ == '__main__':
    generate_notebook()