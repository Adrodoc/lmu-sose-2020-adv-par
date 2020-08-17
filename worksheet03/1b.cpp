#include <iostream>
#include <thread>

class PetersonLock
{
	bool turn = false;
	bool interested[2] = {};
	PetersonLock(const PetersonLock&) = delete;
public:
	PetersonLock() {}
	void lock(bool self) {
		bool other = !self;
		interested[self] = true;
		turn = other;
		while (interested[other] && turn == other) {
		//	std::cout << self << " waiting to acquire lock\n";
		}
		//std::cout << self << " acquired lock\n";
	}
	void unlock(bool self) {
		//std::cout << self << " released lock\n";
		interested[self] = false;
	}
};

void f1(PetersonLock& l, int& counter) {
	bool self = false;
	for (int i = 0; i < 1000; i++) {
		l.lock(self);
		int c = counter;
		for (int i = 0; i < 1000; i++);
		counter = c + 1;
		l.unlock(self);
	}
}

void f2(PetersonLock& l, int& counter) {
	bool self = true;
	for (int i = 0; i < 1000; i++) {
		l.lock(self);
		int c = counter;
		for (int i = 0; i < 1000; i++);
		counter = c + 1;
		l.unlock(self);
	}
}

int main() {
	std::cout << "main()\n";
	PetersonLock l;
	int c = 0;
	std::thread t1(f1, std::ref(l), std::ref(c));
	std::thread t2(f2, std::ref(l), std::ref(c));
	t1.join();
	t2.join();
	std::cout << "counter=" << c;
}
