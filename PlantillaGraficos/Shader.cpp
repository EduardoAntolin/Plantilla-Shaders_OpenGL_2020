#include "Shader.h"

GLuint Shader::getID() {
	return shaderID;
}

Shader::Shader(const char* rutaVertexShader,
	const char* rutaFragmentShader) {

	//Lectura de archivos
	string codigoVertexShader;
	ifstream vertexShaderStream(
		rutaVertexShader, ios::in
	);
	if (vertexShaderStream.is_open()) {
		string linea;
		while (getline(vertexShaderStream, linea)) {
			codigoVertexShader +=
				linea + "\n";
		}
		vertexShaderStream.close();
	}
	else {
		cout << "No se pudo abrir el archivo: " <<
			rutaVertexShader << endl;
	}

	string codigoFragmentShader;
	ifstream fragmentShaderStream(
		rutaFragmentShader,
		ios::in);
	if (fragmentShaderStream.is_open()) {
		string linea;
		while (getline(fragmentShaderStream, linea)) {
			codigoFragmentShader +=
				linea + "\n";
		}
		fragmentShaderStream.close();
	}
	else {
		cout << "No se pudo arbrir el archivo: " <<
			rutaFragmentShader << endl;
	}

	//Cambio de formato string a char*
	const char* cadenaCodigoVertexShader =
		codigoVertexShader.c_str();
	const char* cadenaCodigoFragmentShader =
		codigoFragmentShader.c_str();

	//1.- Crear el programa de shader
	shaderID = glCreateProgram();

	GLuint vertexShaderID =
		glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID =
		glCreateShader(GL_FRAGMENT_SHADER);

	//2.-Cargar el código del shader
	glShaderSource(vertexShaderID, 1,
		&cadenaCodigoVertexShader, NULL);
	glShaderSource(fragmentShaderID, 1,
		&cadenaCodigoFragmentShader, NULL);

	//3.-Compilar los shaders
	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	//4.-Verificar errores de compilacion
	verificarCompilacion(vertexShaderID);
	verificarCompilacion(fragmentShaderID);

	//5.-Adjuntar los shaders al  programa
	glAttachShader(shaderID, vertexShaderID);
	glAttachShader(shaderID, fragmentShaderID);

	//6.- Vincular el programa
	glLinkProgram(shaderID);

	//7.- Verificar si hubo errores en la vinculacion
	verificarVinculacion(shaderID);

	//8.- Usar el programa
	glUseProgram(shaderID);

}

void Shader::verificarCompilacion(GLuint id) {
	GLint resultado;
	GLint longitudLog;

	glGetShaderiv(id, GL_COMPILE_STATUS, &resultado);
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &longitudLog);

	if (resultado == GL_FALSE) {
		cout << "No se pudo compilar shader" << endl;
	}
	if (longitudLog > 0) {
		//Inicia una lista de char con el numero
		//de elementos indicados en longitudLog
		vector<char> mensajeError(longitudLog);
		//Obtener mensajes del compilador y los guarda
		//en la variable mensajeError
		glGetShaderInfoLog(id, longitudLog,
			NULL, &mensajeError[0]);
		//Recorrer el vector e imprimir sus elementos
		for (vector<char>::const_iterator i = mensajeError.begin();
			i != mensajeError.end();
			i++) {
			cout << *i;
		}
		
	}

}

void Shader::verificarVinculacion(GLuint id) {
	GLint estadoVinculacion, estadoValidacion;
	glGetProgramiv(id, GL_LINK_STATUS, &estadoVinculacion);
	if (estadoVinculacion == GL_FALSE) {
		cout << "No se pudo vincular programa" << endl;
	}

	glGetProgramiv(id, GL_VALIDATE_STATUS, &estadoValidacion);
	if (estadoValidacion == GL_FALSE) {
		cout << "No se pudo validar vinculación" << endl;
	}
}

void Shader::enlazar() {
	glUseProgram(shaderID);
}

void Shader::desenlazar() {
	glUseProgram(0);
}