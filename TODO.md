# TODO
- [ ] Actualizar Parser para que utilice los templates de la data.
- [ ] Agregar al Parser la creación de la tabla (Archivo) para cada formato
- [ ] Cuando crea el archivo por primera vez, se agregan todos los datos del CSV (Llamar a funciones readSocialFromCSV y readHospitalFromCSV para obtener la data en un vector)
- [ ] Vincular parser a todas las funciones de los formatos
- [ ] Reemplazar tipo de retorno de todas las funciones que devuelven un valor (ej. find, =, etc.) a json, y utilizar .toJSON() antes de retornar los valores.
- [ ] Llamar Parser desde main (REST API). Parser debe devolver tipo json con los datos.