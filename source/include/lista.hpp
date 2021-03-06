#pragma once
#include <cstddef>
#include <stdexcept>
#include <tuple>

template <typename T> class Lista {
  private:
	class nodo {
		T valor;
		nodo *siguiente = nullptr;

	  public:
		explicit nodo(T &valor, nodo *siguiente = nullptr)
		    : valor(valor), siguiente(siguiente) {}
		const T &getValor() const { return valor; }
		T &getValor() { return valor; }
		void setSiguiente(nodo *n) { siguiente = n; }
		nodo *getSiguiente() const { return siguiente; }
	};
	nodo *inicio = nullptr;
	size_t len = 0;

  public:
	Lista() = default;
	explicit Lista(std::initializer_list<T> l): len(l.size()){
		nodo *actual = nullptr;
		for (auto val : l) {
			if (actual) {
				actual->setSiguiente(new nodo(val))
				actual = actual->getSiguiente();
			} else {
				inicio = new nodo(val);
				actual = inicio;
			}
		}
	}
	explicit Lista(const Lista &l) : len(l.len) {
		nodo *otro = l.inicio;
		nodo *actual = inicio;
		for (size_t pos = 0; pos < l.len; pos++) {
			if(actual){
				actual->setSiguiente(new nodo(otro->getValor()));
				actual = actual->getSiguiente();
			}
			else{
				actual = new nodo(otro->getValor());
			}
			otro = otro->getSiguiente();
		}
	}
	explicit Lista(Lista &&l) noexcept
	    : inicio(l.inicio), len(l.len) {
		l.inicio = nullptr;
		l.len = 0;
	}

	void push_front(T &n) {
		inicio = new nodo(n, inicio);
		len++;
	}
	void push_back(T &n) {
		if (len == 0) {
			insertarInicio(n);
		} else {
			getNodo(len - 1)->setSiguiente(new nodo(n));
			len++;
		}
	}

	void insert(T &n, size_t pos = 0) {
		if (pos == 0) {
			push_front(n);
		} else if (pos <= len) {
			nodo *actual = getNodo(pos - 1);
			actual->setSiguiente(new nodo(n, actual->getSiguiente()));
			len++;
		}
	}
	bool erase(size_t pos = 0) {
		if (pos == 0) {
			return erase_front();
		}
		if (pos < len) {
			nodo *previo = getNodo(pos - 1);
			nodo *sig = previo->getSiguiente()->getSiguiente();
			delete previo->getSiguiente();
			previo->setSiguiente(sig);
			len--;
			return true;
		}
		return false;
	}

	bool erase_front() {
		if (inicio) {
			nodo *siguiente = inicio->getSiguiente();
			delete inicio;
			inicio = siguiente;
			len--;
			return true;
		}
		return false;
	}
	bool erase_back() {
		if (len != 0) {
			if (len == 1) {
				delete inicio;
			} else {
				nodo *n = getNodo(len - 1);
				delete n->getSiguiente();
				n->setSiguiente(nullptr);
			}
			len--;
			return true;
		}
		return false;
	}

	T &at(size_t pos) const {
		nodo *n = getNodo(pos);
		if (n) {
			return n->getValor();
		}
		throw std::out_of_range("posicion fuera de los limites");
	}
	size_t size() const { return len; }
	bool empty() const { return len; }
	void clear() {
		while (len) {
			eliminarInicio();
		}
	}

	T &operator[](size_t pos) const { return at(pos); }
	const T& operator[](size_t pos) const { return at(pos); }

  private:
	nodo *getNodo(size_t pos) const {
		// c++ 17, pre c++17 puede declarar uno de los 2 afuera
		for (auto [n, i] = std::tuple{inicio, 0}; n;
		     i++, n = n->getSiguiente()) {
			if (i == pos) {
				return n;
			}
		}
		return nullptr;
	}

  public:
	class Iterator {
		nodo *actual = nullptr;

	  public:
		Iterator(nodo *n = nullptr) : actual(n) {}

		T &operator*() { return actual->getValor(); }
		bool operator!=(Iterator &o) { return this->actual != o.actual; }

		Iterator &operator++() {
			actual = actual ? actual->getSiguiente() : actual;
			return *this;
		}
	};
	class ConstIterator {
		nodo *actual = nullptr;

	  public:
		ConstIterator(nodo *n = nullptr) : actual(n) {}

		const T &operator*() { return actual->getValor(); }
		bool operator!=(ConstIterator &o) { return this->actual != o.actual; }

		ConstIterator &operator++() {
			actual = actual ? actual->getSiguiente() : actual;
			return *this;
		}
	};

	Lista operator=(const Lista &l) {
		clear();

		nodo *otro = l.inicio;
		nodo *actual = inicio;
		for (size_t pos = 0; pos < l.len; pos++) {
			if (actual) {
				actual->setSiguiente(new nodo(otro->getValor()));
				actual = actual->getSiguiente();
			} else {
				actual = new nodo(otro->getValor());
			}
			otro = otro->getSiguiente();
		}
		return *this;
	}
	Lista operator=(Lista &&l) noexcept {
		clear();
		inicio = l.inicio;
		len = l.len;
		l.inicio = nullptr;
		l.len = 0;
		return *this;
	}

	Iterator begin() { return Iterator(inicio); }
	Iterator end() { return Iterator(getNodo(len - 1)); }

	~Lista() { clear(); }
};
