Gpx speed editor - edits speed of .gpx files... 
made using pugixml, implot and imgui using opengl backend

Choose .gpx file with "Choose File button". Then load it in, with "Load File" button.  
- Manipulate single point, by dragging it. ![single_dot_manipulation](readme_media/single_dot_manipulation.gif)
- Or hold ctrl to 'draw' the curve yourself. ![curve_drawing_mode](readme_media/curve_drawing_mode.gif)

After you are done, just press "Save File" button and it will save changes in current open file

---
Example workflow
![full workflow example](readme_media/full_work_flow.gif)

---

To build you would need
- pugixml > 1.11 (if its lower you would need to update targets in cmake for pugixml from pugixml::pugixml to pugixml(but this was not tested))
- glfw3
- opengl