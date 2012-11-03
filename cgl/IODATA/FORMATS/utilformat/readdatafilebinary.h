#ifndef READDATAFILEBINARY_H
#define READDATAFILEBINARY_H

// inclusiones especificas
#include <fstream>
#include <string>

#include "readdatabinary.h"

CGL_BEGIN

/** Lee datos de un archivo en forma binaria
*/
class ReadDataFileBinary : public ReadDataBinary
{
	public:
		
		/** Constructor por omisión
		*/
		ReadDataFileBinary()
		{
		}

		/** Construye un lector de datos a partir de un archivo
		*/
		ReadDataFileBinary(const std::string& pathFile)
		{
			m_file.open(pathFile.data(),ios::in|ios::binary);
		}

		/** SetFile
		*/
		void setFile(const std::string& pathFile)
		{
			close();
			m_file.open(pathFile.data(),ios::in|ios::binary);
		}		
		
		/** Destructor
		*/
		~ReadDataFileBinary()
		{
			close();
		}
		
		/** Lee datos
		*/
		void read(void* dest, const int& len)
		{
			m_file.read((char*)dest,len);
		}
		
		/** Fija la posición en los datos
		*/
		void setpos(const unsigned long& p)
		{
			m_file.seekg(p,ios::beg);
		}
		
		/** Retorna la posición en los datos
		*/
		unsigned long getpos(void)
		{
			return m_file.tellg();
		}

		/** Cierra el archivo binario
		*/
		void close(void)
		{
			if(m_file.is_open())
			{
				m_file.close();
			}
		}

		/** 
		*/
		bool ready()
		{
			return m_file.is_open();
		}

	private:
		// Apuntador al archivo
		fstream m_file;
};

CGL_END

#endif