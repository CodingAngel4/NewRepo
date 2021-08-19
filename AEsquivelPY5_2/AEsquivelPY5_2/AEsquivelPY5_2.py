import folium
import webbrowser

map1 = folium.Map(location=[41.382, 2.163], zoom_start =10)
folium.CircleMarker(location=[41.382, 2.163], radius=150, popup="Barcelona").add_to(map1)
folium.Marker([41.382, 2.163]).add_to(map1)
folium.Marker([41.381, 2.122], popup='Camp Nou').add_to(map1)

folium.PolyLine(locations = [(41.382, 2.163), (41.381, 2.122)], line_opacity=.6).add_to(map1)

map1.save("Barca.html")

#open in browser
webbrowser.open("Barca.html")