/*
	uHDL.h
	uHDL v0.2 by Slavko Novak [slavko.novak.esen@gmail.com], January 2015
	License: LGPL
*/

#ifndef _U_HDL_H
#define _U_HDL_H

namespace uHDL
{	
	//Macros -->
	
	#define uHDL_VER = 0.2
	
	#define uMODULE(module_name) struct module_name : public uHDL::uModule
	#define uINIT(module_name) module_name()
	#define uALWAYS(sensitive, method) void Always() { if(sensitive) { method } }
	#define uMODULES(modules_list_name, modules_count) uHDL::uArray<uModule*, modules_count> modules_list_name;
	#define uDO_ON_tick(ticks) if(ticks == uHDL::TICKS)
	#define uDO_ON_ms(milli_seconds) if(milli_seconds == millis())
	#define uDO_ON_us(micro_seconds) if(micro_seconds == micros())
	#define uEXIT DoStimulation = false;
		
	#define POS_EDGE 1
	#define NEG_EDGE -1
	#define NONE_EDGE 0
	
	#define u_byte uint8_t 
	
	//Macros <--
	
	//***************************************************
	
	//Util type templates -->
	
	template <typename T, unsigned int SIZE = 1>
	class uArray
	{
		unsigned int cursor;
		unsigned int count;
		T data[SIZE];
	public:
		uArray(): count(0)
		{
			for(int i = 0; i < SIZE; i++) this->data[i] = T();
		}
		
		void Add(T data)
		{
			this->data[this->count++] = data;
		}
		
		T Get(unsigned int index)
		{
			return this->data[index];
		}
		
		void Set(unsigned int index, T data)
		{
			this->data[index] = data;
		}
		
		T& RW(unsigned int index)
		{
			return this->data[index];
		}
		
		int Count() const
		{
			return this->count;
		}
		
		T* Next()
		{
			T *ret = (T *)0;
			
			if(this->cursor < this->count)
			{
				ret = &this->data[cursor++];
			}
			else
			{
				cursor = 0;
			}
			
			return ret;
		}
	};
	
	//<-- Util type templates
	
	//***************************************************
	
	//Wireing type templates -->
		
	template <typename T = bool>
	class u_port
	{
	protected:
		T data;
    signed char data_state;
 		bool data_change;
		
		#ifdef U_HDL_WIREING
		u_port<T> *wireTo;
		#endif
		
	public:
		u_port() : data(T()), data_state(0), data_change(false)
				#ifdef U_HDL_WIREING
					, wireTo((u_port<T> *)0)
				#endif
		{}
		
		T& Read()
		{
			return this->data;
		}
		
		virtual void Write(const T& data)
		{
			#ifdef U_HDL_WIREING
			if(this->wireTo)
			{
				this->wireTo->Write(data);
			}
			#endif
				
			if(data != this->data)
			{
				this->data_change = true;
				this->data_state = data > this->data ? POS_EDGE : NEG_EDGE;
			}
			else this->data_change = false;
			
			this->data = data;
		}
		
		bool Change()
		{
			bool ret_val = this->data_change;
			this->data_change = false;
			return ret_val;
		}
		
		bool PosEdge()
		{
			bool ret_val = POS_EDGE == this->data_state;
			this->data_state = NONE_EDGE;
			return ret_val;
		}
		
		bool NegEdge()
		{
			bool ret_val = NEG_EDGE == this->data_state;
			this->data_state = NONE_EDGE;
			return ret_val;
		}
		
		#ifdef U_HDL_WIREING
		void WireTo(u_port<T> *wireTo)
		{
			this->wireTo = wireTo;
		}
		#endif
	};
	
	template <typename T = bool, unsigned int SIZE = 1>
	class u_signal : public u_port<T>
	{
		uArray<u_port<T>*, SIZE> inputs;
		uArray<u_port<T>*, SIZE> outputs;
	public:
		u_signal(): inputs(uArray<u_port<T>*, SIZE>()), outputs(uArray<u_port<T>*, SIZE>())
		{}
				
		void _bind(u_port<T> *data, bool input)
		{
			if(input) this->inputs.Add(data);
			else this->outputs.Add(data);
		}
		
		void Write(const T& data)
		{
			for(int i = 0; i < this->inputs.Count(); i++) this->inputs.Get(i)->Write(data);
		}
		
		T& Read(unsigned int index = 0)
		{
			if(this->inputs.Count()) return inputs.Get(index)->Read(); else return this->data;
		}
	};
	
	template <typename T = bool>
	class u_input : public u_port<T>
	{
		u_signal<T> *signal;
	public:
		u_input() : signal((u_signal<T> *)0)
		{}
		
		void Connect(u_signal<T> *signal)
		{
			this->signal = signal;
			if(this->signal) this->signal->_bind(this, true);
		}
	};
	
	template <typename T = bool>
	class u_output : public u_port<T>
	{
		u_signal<T> *signal;
	public:
		T Read() const
		{
			return this->data;
		}
		
		void Connect(u_signal<T> *signal)
		{
			this->signal = signal;
			if(this->signal) this->signal->_bind(this, false);
		}
		
		void Write(const T& data)
		{
			u_port<T>::Write(data);
			if(this->signal) this->signal->Write(data);
		}
	};
	
	template <typename T = bool>
	class u_reg : public u_port<T>
	{
	public:
		T& RW()
		{
			return this->data;
		}
	};
	
	//<-- Wireing type templates
	
	//***************************************************
	
	//Modul primitives -->
	
	class uModule
	{
	public:
		virtual void Always()
		{}
	};
	
	//<-- Modul primitives
	
	//***************************************************
	
	//Engine methods and objects-->
	
	extern unsigned long TICKS;
	
	//This method is implemented in uHDL.cpp file
	void uMain(const int modc, uModule **modref);
	
	template <typename T>
	void uMain(T modules)
	{
		/*
		for(int i = 0; i < modules.Count(); i++)
		{
			modules.Get(i)->Always();
		}
		*/
		
		while(uModule **module = modules.Next())
		{
			(*module)->Always();
		}
		
		TICKS++;
	}
	
	
	//<-- Engine methods
	
	//***************************************************
	
	//Util methods -->
	template <typename T>
	bool uGetBit(const T& data, const unsigned int bit)
	{
		return data & ((T)1 << bit);
	}
	//<-- Util methods
}

#endif
