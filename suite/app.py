import streamlit as st
import pandas as pd
import plotly.express as px
from data_processor import fetch_team_data
from PIL import Image, ImageDraw
import os

st.set_page_config(page_title="Pumas Sangrientos Suite", layout="wide")

# --- FUNCIÓN PARA EL PODIO ---
def generar_imagen_podio(ranking, handles):
    # Dimensiones y fondo
    w, h = 800, 500
    img = Image.new('RGB', (w, h), (30, 30, 30))
    draw = ImageDraw.Draw(img)
    
    # Coordenadas: [1ero (Oro), 2do (Plata), 3ero (Bronce)]
    podio_coords = [
        (300, 200, 500, 500, (255, 215, 0)),  # Centro
        (100, 300, 300, 500, (192, 192, 192)), # Izquierda
        (500, 350, 700, 500, (205, 127, 50))   # Derecha
    ]
    
    # Solo tomamos los top 3
    puestos = ranking[:3]
    orden_dibujo = []
    if len(puestos) >= 1: orden_dibujo.append((puestos[0], podio_coords[0]))
    if len(puestos) >= 2: orden_dibujo.append((puestos[1], podio_coords[1]))
    if len(puestos) >= 3: orden_dibujo.append((puestos[2], podio_coords[2]))

    for (user_data, coords) in orden_dibujo:
        handle, total = user_data
        x1, y1, x2, y2, color = coords
        draw.rectangle([x1, y1, x2, y2], fill=color, outline=(255,255,255))
        
        # Intentar cargar foto (busca en la raíz de la carpeta suite)
        foto_path = f"{handle}.png"
        if os.path.exists(foto_path):
            try:
                user_img = Image.open(foto_path).convert("RGBA")
                user_img = user_img.resize((150, 150))
                img.paste(user_img, (x1 + (x2-x1-150)//2, y1 - 160), user_img)
            except: pass
        
        # Nombre y Total
        draw.text((x1 + 60, y1 + 20), f"{handle}\n{total} Solved", fill="black")
    
    return img

# --- SIDEBAR ---
st.sidebar.header("⚙️ Configuración")
api_key = st.sidebar.text_input("API Key", type="password", value="7516f7087ac2f3219ec3646c2d8f593f216aa0c4")
secret = st.sidebar.text_input("Secret", type="password", value="d4957efc266b37315b1934608efe2e7730335c0e")
group_code = st.sidebar.text_input("Group Code", value="V0tkorRDn2")
handles_input = st.sidebar.text_area("Handles", value="Moncho, movaa, botellopai23")
handles = [h.strip() for h in handles_input.split(",")]

contests_config = {
    "685299": "L", "684722": "L", "683540": "K", "681278": "L",
    "679889": "M", "678033": "N", "676748": "L", "674806": "L"
}

st.title("🐾 Pumas Sangrientos ICPC Suite")

if st.sidebar.button("🔄 Actualizar Datos"):
    data = fetch_team_data(api_key, secret, group_code, handles, contests_config)
    
    if data:
        # CREACIÓN DE PESTAÑAS
        tab1, tab2, tab3 = st.tabs(["📊 Dashboard", "🏆 Podio de Honor", "📂 Detalles por Contest"])

        with tab1:
            st.subheader("Progreso del Grupo")
            progress = data['total_solved_unique'] / data['total_gym_problems']
            st.progress(progress)
            st.write(f"**{data['total_solved_unique']} / {data['total_gym_problems']}** problemas conquistados.")

            chart_data = []
            for h in handles:
                h_l = h.lower()
                chart_data.append({"User": h, "Tipo": "Contest", "Cantidad": len(data['user_stats'][h_l]['contest'])})
                chart_data.append({"User": h, "Tipo": "Upsolve", "Cantidad": len(data['user_stats'][h_l]['upsolve'])})
            
            df = pd.DataFrame(chart_data)
            fig = px.bar(df, x="User", y="Cantidad", color="Tipo", barmode="group", color_discrete_map={'Contest':'#4CAF50', 'Upsolve':'#2196F3'})
            st.plotly_chart(fig, use_container_width=True)

        with tab2:
            st.subheader("Los Mejores de la Suite")
            # Preparar ranking para el podio
            ranking = []
            for h in handles:
                total = len(data['user_stats'][h.lower()]['contest']) + len(data['user_stats'][h.lower()]['upsolve'])
                ranking.append((h, total))
            ranking.sort(key=lambda x: x[1], reverse=True)
            
            # Generar y mostrar imagen
            podio_img = generar_imagen_podio(ranking, handles)
            st.image(podio_img, caption="Ranking basado en Problemas Totales (Contest + Upsolve)")
            
            # Tabla simple debajo
            st.table(pd.DataFrame(ranking, columns=["Usuario", "Total Solved"]))

        with tab3:
            for rep in data['contest_reports']:
                with st.expander(f"🏆 {rep['name']} (ID: {rep['id']})"):
                    c1, c2, c3 = st.columns(3)
                    c1.success(f"✅ Contest: {' '.join(rep['solved']) or '-'}")
                    c2.info(f"🆙 Upsolve: {' '.join(rep['upsolved']) or '-'}")
                    c3.error(f"❌ Faltan: {' '.join(rep['missing']) or '-'}")
    else:
        st.error("Error al obtener datos. Revisa la consola o las credenciales.")