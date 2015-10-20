int main()
{
    int k = 5, *j;
	j = k;
	devolve(j);
	printf("%i\n", k);
	return 0;
}

void devolve(int *k){
	*k++;
	printf("%i lala\n",                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            k);
}
