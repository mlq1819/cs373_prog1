#ifndef MQ_FILEREADER_H
#define MQ_FILEREADER_H
#include <stdlib.h>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <vector>

class FileReader{
	private:
		std::ifstream * file;
		std::vector<std::string> lines;
		bool read;
		std::size_t index;
	public:
		FileReader(std::ifstream *);
		~FileReader(){file=NULL;};
		bool isRead() const {return this->read;};
		bool readFile();
		std::string operator[](std::size_t index) const {return this->lines[index];};
		bool atStart() const {return this->index==0;};
		bool atEnd() const {return this->index==this->getSize()-1;};
		bool next();
		std::string current() const {return this->lines[this->index];};
		void start(){this->index=0;};
		void end(){this->index=this->getSize()-1;};
		std::size_t getIndex() const {return this->index;};
		std::size_t getSize() const {return this->lines.size();};
};

#endif
