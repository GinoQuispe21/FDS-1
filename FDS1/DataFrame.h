#pragma once
#include "Column.h"
#include "Row.h"
#include <vector>
#include "Arbolito.h"
using namespace std;

class DataFrame {
private:
	map<string, AVLTree<Row*,string>*> arboles;
	vector<Row*>* Filas;
	map<string, Column*>* Columnas;
	vector<string> nombres;
public: 
	DataFrame( map<string,Column*>* f = nullptr )  {
		Columnas = new map<string, Column*>;
		Filas = new vector<Row*>;
	}
	~DataFrame() {
		for (auto a : *Filas) {
			delete a;
		}
		delete Filas;
		for (auto b : nombres) {
			delete (*Columnas)[b];
		}
		delete Columnas;
		for (auto c : nombres) {
			if (ConsultarIndx(c)) {
				delete arboles[c];
			}
		}
	}
	void setNombres(vector<string> nms) {
		nombres = nms;
		for (int i = 0; i < nombres.size(); i++) {
			(*Columnas)[nombres[i]] = new Column();
		}
	}
	void setNomb(vector<string>nms) {
		nombres = nms;
	}
	class iterator {
		unsigned int pos;
		vector<Row*>* f;
	public:

		iterator(vector<Row*>* f, unsigned int pos) : f(f), pos(pos) {}
		iterator(vector<Row*>* f) : pos(0) {}
		bool operator != (iterator other) {
			return this->pos != other.pos;
		}
		Row* operator *() {
			return f->at(pos);
		}
		void operator ++() {
			pos++;
		}
	};


	iterator begin() {
		return iterator(Filas, 0);
	}
	iterator end() {
		return iterator(Filas, Filas->size());
	}
	DataFrame* select(vector<string> colNames) {
		map<string, Column*>* nCols = new map<string, Column*>;
		DataFrame* nDF = new DataFrame(nCols);
		nDF->setNombres(colNames);
		for (auto cn : colNames) {
			nDF->Columnas->at(cn) = Columnas->at(cn);
		}
		
		nDF->Filas = Filas;
		return nDF;
	}

	DataFrame* filter(string nc1, string op1, string val1, string nc2 = "", string op2 = "", string val2 = "") {
		DataFrame* nDF = new DataFrame();
		nDF->setNombres(nombres);
		nDF->Columnas = Columnas;
		for (auto r : *Filas) {		
			if (nc2 == "" && compare(op1, r->getData(nc1), val1) ) {
				nDF->Filas->push_back(r);
			}
			else if(compare(op1, r->getData(nc1), val1) && compare(op2, r->getData(nc2), val2)) {
				nDF->Filas->push_back(r);
			}
		}
		return nDF;
	}
	DataFrame* sort(string colName) {
		DataFrame* nDF = new DataFrame();
		nDF->setNombres(nombres);
		nDF->Columnas = Columnas;
		vector<Row*>* f = arboles[colName]->Ordenar();
		nDF->Filas = f;
		return nDF;
	}
	bool ConsultarIndx(string colname) {
		if (arboles.find(colname) != arboles.end()) {
			return true;
		}
		else return false;
	}
	vector<Row*>* Buscarf(string colname, string elem) {
		vector<Row*>* vec = arboles[colname]->Buscar(dummy(colname, elem));
		return vec;
	}
	Row* dummy(string colname, string elem) {
		map<string, Column*>* f = new map<string, Column*>;
		(*f)[colname] = new Column;
		(*f)[colname]->añadirDatos(elem);
		Row* wa = new Row(f,0);
		return wa;

	}
	void indexar(string colName) {	
		AVLTree<Row*, string>* arb = new AVLTree<Row*, string>([=](Row* r) {
			return r->getData(colName);
		});	
		arb->setTipo((*Filas)[0]->getData(colName));
		for (auto row : *Filas) {
	
			arb->Add(row);
		}
		arboles[colName] = arb;
		//arb->inord();
	}
	void añadirFila(vector<string> a) {
		int n = a.size();
		for (int i = 0; i < nombres.size(); i++) {
			string b = nombres[i];
			(*Columnas)[b]->añadirDatos(a[i]);			
		}
		int fu = Filas->size();
		Filas->push_back(new Row(Columnas, fu));
	}

	bool subt(string a, string b) {
		if (a.find(b) != std::string::npos) return true;
		else return false;
	}
	bool compare(string op, string b, string f) { // f sera la el elemento guia
		switch (op[0]) {
		case '>':
			if (stoll(b) > stoll(f)) return true;
			break;
		case '<':
			if (stoll(b) < stoll(f)) return true;
		case '=':
			if (b == f) return true;
			break;
		case '0':
			if (b[0] == f[0]) return true;
			break;
		case 'n':
			if (b[b.size() - 1] == f[f.size() - 1]) return true;
			break;
		case 'z':
			if (subt(b, f)) return true;
			break;
		case 'x':
			if (!subt(b, f)) return true;
			break;
	}
		return false;
	}

	vector<Row*>* getFilas() {
		return this->Filas;
	}
	vector<string> getNombres() {
		return nombres;
	}
	void añadirNombre(string f) {
		nombres.push_back(f);
	}
};
