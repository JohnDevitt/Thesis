
template <class data_class> 
void gQuickSort(data_class* data, int off, int len) {
	     
	if (len < 7) {
	    for (int i=off; i<len+off; i++)
		for (int j=i; j>off && data[j-1]<=data[j]; j--)
		    swap(data, j, j-1);
	    return;
	}

	     
	int m = off + len/2;           
	if (len > 7) {
	    int l = off;
	    int n = off + len - 1;
	    if (len > 40) {             
		int s = len/8;
		l = med3(data, l,     l+s, l+2*s);
		m = med3(data, m-s,   m,   m+s);
		n = med3(data, n-2*s, n-s, n);
	    }
	    m = med3(data, l, m, n);     
	}
	double v = data[m];

	      
	int a = off, b = a, c = off + len - 1, d = c;
	while(true) {
	    while (b <= c && data[b] > v) {
		if (data[b] == v)
		    swap(data, a++, b);
		b++;
	    }
	    while (c >= b && data[c] < v) {
		if (data[c] == v)
		    swap(data, c, d--);
		c--;
	    }
	    if (b > c)
		break;
	    swap(data, b++, c--);
	}

	      
	int s, n = off + len;
	s = min(a-off, b-a  );  vecswap(data, off, b-s, s);
	s = min(d-c,   n-d-1);  vecswap(data, b,   n-s, s);

	   
	if ((s = b-a) > 1)
	    gQuickSort(data, off, s);
	if ((s = d-c) > 1)
	    gQuickSort(data, n-s, s);
}


     
 
template <class data_class> 
void swap(data_class* data, int a, int b) {
	data_class t = data[a];
	data[a] = data[b];
	data[b] = t;
}


         
 
template <class data_class>  
void vecswap(data_class* data, int a, int b, int n) {
	for (int i=0; i<n; i++, a++, b++) swap<data_class>(data, a, b);
}


            
 
template <class data_class>   
int med3(data_class* data, int a, int b, int c) {
	return (data[a] >= data[b] ?
		(data[b] >= data[c] ? b : data[a] >= data[c] ? c : a) :
		(data[b] <= data[c] ? b : data[a] <= data[c] ? c : a));
}

