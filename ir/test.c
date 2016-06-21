int main() {
	int i;
	for(i = 0; i < 1000; ++i) {
		if(i % 3 == 0 && i % 5 == 0) {
			printf("%d is divisible by 3 and 5.\n", i);
		}
	}
}
