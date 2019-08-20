# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# define AUTHKEY "ac85a7d9"
 
typedef struct tag //estrutura de tag
{
	char tag[1001];
	char key[51];
	int hit;
}tag;

typedef struct listaTag //lista que contém o vetor de struct tags
{
	tag *v;
	int max;
	int qntd;
}listaTag;

typedef struct key //estrutura  de key
{
	char key[51];
	char conteudo[1001];
	int ref;
}key;

typedef struct listaKey //lista que contém o vetor de struct chaves
{
	key *v;
	int max;
	int qntd;
}listaKey;

void tagHit(char *str1, char *str2, listaTag *t, listaKey *l);
void addKey(char *str1, char *str2, listaKey *l);
void inicializaKey(listaKey *l);
void inicializaTag(listaTag *t);
void realocaKey(listaKey *l);
void realocaTag(listaTag *t);
void dumpKeys(listaKey *l);
void dumpTags(listaTag *t);
int verificaKey(listaKey *l);
int separaKey (key *v, int p, int r);
void quicksortKey (key *v, int p, int r);
int separaTag (tag *v, int p, int r);
void quicksortTag (tag *v, int p, int r);
int verificaTag(listaTag *t);
int buscaKey (char *x, int n, key *v);
int buscaTag (char *x, int n, tag *v);
void showTag(listaTag *t, char *str, listaKey *l);
void newDay(listaTag *t, listaKey *l);
void quicksortHit (tag *v, int p, int r);
int separaHit (tag *v, int p, int r);
void listTop(listaTag *t, int xx);
int verificaHit(listaTag *t);
void listBot(listaTag *t, int xx);
void colocapivoKey(key *v, int p, int r);
void colocapivoTag(tag *v, int p, int r);
void colocapivoHit(tag *v, int p, int r);
int separaTagInv (tag *v, int p, int r);
void quicksortTagInv (tag *v, int p, int r);

void addKey(char *str1, char *str2, listaKey *l) //função que adiciona a chave
{
	if(l->qntd == l->max)
		realocaKey(l);
	int tam;
	tam = strlen(str1);
	str1[tam-1] = '\0'; //exclui os dois pontos do str1
	strcpy(l->v[l->qntd].key, str1);
	strcpy(l->v[l->qntd++].conteudo, str2);
	l->v[l->qntd - 1].ref = 0;
}

void tagHit(char *str1, char *str2, listaTag *t, listaKey *l) //função para adicionar as tags
{
	if(t->qntd == t->max)
		realocaTag(t);

	if(t->qntd == 0) //se não houver tags, simplesmente adiciona, não faz buscas
	{
		int i;
		strcpy(t->v[t->qntd].tag, str1);
		strcpy(t->v[t->qntd++].key, str2);
		if(verificaKey(l) == 0)
				quicksortKey(l->v, 0, l->qntd-1);
		i = buscaKey(t->v[t->qntd-1].key, l->qntd, l->v);
		l->v[i].ref++;
		t->v[t->qntd - 1].hit = 1;
	}
	else
	{
		if(verificaTag(t) == 0)
			quicksortTag(t->v, 0, t->qntd-1);

		int i;
		i = buscaTag(str1, t->qntd, t->v);
		
		if(i == -1)
		{
			int j;
			strcpy(t->v[t->qntd].tag, str1);
			strcpy(t->v[t->qntd++].key, str2);
			if(verificaKey(l) == 0)
				quicksortKey(l->v, 0, l->qntd-1);
			j = buscaKey(t->v[t->qntd-1].key, l->qntd, l->v);
			l->v[j].ref++;
			t->v[t->qntd - 1].hit = 1;
		}
		else if(strcmp(str2, t->v[i].key) == 0)
			t->v[i].hit++;
	}
}

void inicializaKey(listaKey *l) //função para inicializar as chaves
{
	l->v = (key*)malloc(100*sizeof(key));
	l->max = 100;
	l->qntd = 0;
}

void inicializaTag(listaTag *t) //função para inicializar as tags
{
	t->v = (tag*)malloc(100*sizeof(key));
	t->max = 100;
	t->qntd = 0;
}

void realocaKey(listaKey *l) //função para realocar a memória das keys quando atinge a quantidade máxima
{
	l->v = (key*)realloc(l->v, (l->max*2)*sizeof(key));
	l->max = 2*l->max;
}

void realocaTag(listaTag *t) //função para realocar a memória das tags quando atinge a quantidade máxima
{
	t->v = (tag*)realloc(t->v, (t->max*2)*sizeof(tag));
	t->max = 2*t->max;
}

void dumpKeys(listaKey *l) //função que printa as chaves, seus conteúdos e suas respectivas referências
{
	int i;

	if(verificaKey(l) == 0)
		quicksortKey(l->v, 0, l->qntd-1);

	printf("8<----------Begin Key Dump----------\n");
	for(i=0; i<l->qntd; i++)
		printf("%s content=\"%s\" refs=%d\n", l->v[i].key, l->v[i].conteudo, l->v[i].ref);
	printf("8<----------End   Key Dump----------\n");
}

void dumpTags(listaTag *t) //função que printa as tags, seus conteúdos e seus respectivos hits
{
	int i;

	if(verificaTag(t) == 0)
		quicksortTag(t->v, 0, t->qntd-1);
	
	printf("8<----------Begin Tag Dump----------\n");
	for(i=0; i<t->qntd; i++)
		printf("%s -> %s :: hits=%d\n", t->v[i].tag, t->v[i].key, t->v[i].hit);
	printf("8<----------End   Tag Dump----------\n");
	
}

int verificaKey(listaKey *l) //verifica se o vetor de chaves está ordenado lexicograficamente
{
	int i, x;
	for(i=0; i<l->qntd-1; i++)
	{
		x = strcmp(l->v[i].key, l->v[i+1].key);
		if(x>0)
			return 0;
	}
	return 1;
}

int verificaTag(listaTag *t) //verifica se o vetor de tags está ordenado lexicograficamente
{
	int i, x;
	for(i=0; i<t->qntd-1; i++)
	{
		x = strcmp(t->v[i].tag, t->v[i+1].tag);
		if(x>0)
			return 0;
	}
	return 1;
}

void showTag(listaTag *t, char *str, listaKey *l) //mostra o conteúdo das tags
{
	int i;
	if(verificaTag(t) == 0)
		quicksortTag(t->v, 0, t->qntd-1);

	i = buscaTag(str, t->qntd, t->v);
	printf("%s -> %s\n", t->v[i].tag, t->v[i].key);

	if(verificaKey(l) == 0)
		quicksortKey(l->v, 0, l->qntd-1);

	i = buscaKey(t->v[i].key, l->qntd, l->v);
	printf("%s :. %s\n", l->v[i].key, l->v[i].conteudo);
}

int separaKey (key *v, int p, int r) //algoritmo de ordenação adaptado para ordenar lexicograficamente
{
   int i = p + 1, j = r; 
   key c, t;
   colocapivoKey(v, p, r);
   c = v[p]; 
   while (i <= j) {
      if(strcmp(v[i].key, c.key) <= 0) i++;
      else if(strcmp(c.key, v[j].key) < 0) j--;
      else {
         t = v[i], v[i] = v[j], v[j] = t; 
         i++, j--;
      }
   }   
   v[p] = v[j], v[j] = c;
   return j; 
}

 
void quicksortKey (key *v, int p, int r) //algoritmo de ordenação adaptado para ordenar lexicograficamente
{
   int j;                         
   if (p < r) {                   
      j = separaKey (v, p, r);       
      quicksortKey (v, p, j-1);      
      quicksortKey (v, j+1, r);      
   }
}

int separaTag (tag *v, int p, int r) //algoritmo de ordenação adaptado para ordenar lexicograficamente
{
   int i = p+1, j = r; 
   tag c, t;
   colocapivoTag(v, p, r);
   c = v[p]; 
   while (i <= j) {
      if(strcmp(v[i].tag, c.tag) <= 0) i++;
      else if(strcmp(c.tag, v[j].tag) < 0) j--;
      else {
         t = v[i], v[i] = v[j], v[j] = t; 
         i++, j--;
      }
   }   
   v[p] = v[j], v[j] = c;
   return j; 
}

 
void quicksortTag (tag *v, int p, int r) //algoritmo de ordenação adaptado para ordenar lexicograficamente
{
   int j;                         
   if (p < r) {                   
      j = separaTag (v, p, r);       
      quicksortTag (v, p, j-1);      
      quicksortTag (v, j+1, r);      
   }
}

 
int buscaKey (char *x, int n, key *v) //algoritmo de busca adaptado para fazer buscas em vetores ordenados lexicograficamente
{
   int e, m, d;                              
   e = 0; d = n-1;                           
   while (e <= d) 
   {                          
      m = (e + d)/2;                         
      if(strcmp(v[m].key, x) == 0) return m;              
      if(strcmp(v[m].key, x) < 0) e = m + 1;
      else d = m - 1;                        
   }                                         
   return -1;                                
}           

int buscaTag (char *x, int n, tag *v) //algoritmo de busca adaptado para fazer buscas em vetores ordenados lexicograficamente
{
   int e, m, d;                              
   e = 0; d = n-1;                           
   while (e <= d) 
   {                          
      m = (e + d)/2;                         
      if(strcmp(v[m].tag, x) == 0) return m;              
      if(strcmp(v[m].tag, x) < 0) e = m + 1;
      else d = m - 1;                        
   }                                         
   return -1;                                
}

void newDay(listaTag *t, listaKey *l) //função que começa um novo dia. Os hits das tags são zerados
{
	if(verificaKey(l) == 0)
		quicksortKey(l->v, 0, l->qntd-1);

	int i, j;
	for(i=0; i<t->qntd; i++)
	{
		if(t->v[i].hit != 0)
			t->v[i].hit = 0;
		else
		{
			int k;
			k = buscaKey(t->v[i].key, l->qntd, l->v);
			l->v[k].ref--;
			for(j=i; j<t->qntd; j++)
				t->v[j] = t->v[j+1];
			i--;
			t->qntd--;
		}
	}
}

int separaHit (tag *v, int p, int r) //algoritmo de ordenação para ordenar por quantidade de hits
{
	colocapivoHit(v, p, r);
   int c = v[p].hit, i = p+1, j = r;
   tag t;
   while (i <= j) {
      if (v[i].hit <= c) ++i;
      else if (c < v[j].hit) --j; 
      else {
         t = v[i], v[i] = v[j], v[j] = t;
         ++i; --j;
      }
   }
   tag aux;               
   aux = v[p], v[p] = v[j], v[j] = aux;
   return j; 
}

void quicksortHit (tag *v, int p, int r) //algoritmo de ordenação para ordenar por quantidade de hits
{
   int j;                         
   if (p < r) {                   
      j = separaHit (v, p, r);       
      quicksortHit (v, p, j-1);      
      quicksortHit (v, j+1, r);
   }
}

int verificaHit(listaTag *t) //verifica se o vetor de tags está ordenado por quantidade de hits
{
	int i;
	for(i=0; i<t->qntd; i++)
		if(t->v[i].hit > t->v[i+1].hit)
			return 0;
	return 1;
}

void listTop(listaTag *t, int xx) //lista os xx% das tags com mais quantidade de hits
{
	int n = (xx*t->qntd)/100, pos=1;
	int i, ini, fim;

	if(verificaHit(t) == 0)
		quicksortHit(t->v, 0, t->qntd-1);
	for(i=0; i<t->qntd - 1; i++)
	{
		ini = i, fim = i;
		while(t->v[i].hit == t->v[i+1].hit && i < t->qntd)
		{
			fim++;
			i++;
		}
		if(ini != fim)
		{
			quicksortTagInv(t->v, ini, fim);
		}
	}


	printf("Begin %d%% top trending\n", xx);
	i = t->qntd-1;
	while(n>0)
	{		
		printf("%-3d %s with %d hits\n", pos, t->v[i].tag, t->v[i].hit);
		if(t->v[i].hit != t->v[i-1].hit)
			pos++;	
		
		i--;
		n--;
	}
	printf("End Trending\n");
}

void listBot(listaTag *t, int xx) //lista os xx% das tags com menos quantidade de hits
{
	int n = (xx*t->qntd)/100, pos = t->qntd;
	int i, ini, fim;

	if(verificaHit(t) == 0)
		quicksortHit(t->v, 0, t->qntd-1);
	for(i=0; i<t->qntd - 1; i++)
	{
		ini = i, fim = i;
		while(t->v[i].hit == t->v[i+1].hit)
		{
			fim++;
			i++;
		}
		if(ini != fim)
		{
			quicksortTagInv(t->v, ini, fim);
		}
	}

	printf("Begin %d%% bottom trending\n", xx);
	i = 0;
	while(n>0)
	{
		printf("%-3d %s with %d hits\n", pos, t->v[i].tag, t->v[i].hit);
		if(t->v[i].hit != t->v[i+1].hit)
			pos--;

		i++;
		n--;
	}
	printf("End Trending\n");
}


void colocapivoKey(key *v, int p, int r) //melhora o quicksort para keys
{
	int a, b, c, n;
	a = lrand48()%(r-p)+p;
	b = lrand48()%(r-p)+p;
	c = lrand48()%(r-p)+p;

	if(strcmp(v[a].key, v[b].key) > 0)
		n = a, a = b, b = n;
	if(strcmp(v[b].key, v[c].key) > 0)
		n = b, b = c, c = n;
	if(strcmp(v[a].key, v[b].key) > 0)
		n = a, a = b, b = n;

	key aux;
	aux = v[p], v[p] = v[b], v[b] = aux;
}

void colocapivoTag(tag *v, int p, int r) //melhora o quicksort para tags quando é preciso ordenar lexicograficamente
{
	int a, b, c, n;
	a = lrand48()%(r-p)+p;
	b = lrand48()%(r-p)+p;
	c = lrand48()%(r-p)+p;

	if(strcmp(v[a].tag, v[b].tag) > 0)
		n = a, a = b, b = n;
	if(strcmp(v[b].tag, v[c].tag) > 0)
		n = b, b = c, c = n;
	if(strcmp(v[a].tag, v[b].tag) > 0)
		n = a, a = b, b = n;

	tag aux;
	aux = v[p], v[p] = v[b], v[b] = aux;
}

void colocapivoHit(tag *v, int p, int r) //melhora o quicksort para tags quando é preciso ordenar por hits
{
	int a, b, c, n;
	a = lrand48()%(r-p)+p;
	b = lrand48()%(r-p)+p;
	c = lrand48()%(r-p)+p;

	if(v[a].hit > v[b].hit)
		n = a, a = b, b = n;
	if(v[b].hit > v[c].hit)
		n = b, b = c, c = n;
	if(v[a].hit > v[b].hit)
		n = a, a = b, b = n;

	tag aux;
	aux = v[p], v[p] = v[b], v[b] = aux;
}

int separaTagInv (tag *v, int p, int r) //algoritmo de ordenação adaptado para ordenar de forma invertida
{
   int i = p+1, j = r;
   tag c, t;
   colocapivoTag(v, p, r);
   c = v[p];
   while (i <= j) {
      if(strcmp(v[i].tag, c.tag) > 0) i++;
      else if(strcmp(c.tag, v[j].tag) >= 0) j--;
      else {
         t = v[i], v[i] = v[j], v[j] = t; 
         i++, j--;
      }
   }   
   v[p] = v[j], v[j] = c;
   return j; 
}

 
void quicksortTagInv (tag *v, int p, int r) //algoritmo de ordenação adaptado para ordenar de forma invertida
{
   int j;                         
   if (p < r) {                   
      j = separaTagInv (v, p, r);       
      quicksortTagInv (v, p, j-1);      
      quicksortTagInv (v, j+1, r);      
   }
}

int main()
{
	char str1[1001], str2[1001];
	listaKey l;
	listaTag t;

	inicializaKey(&l);
	inicializaTag(&t);
	
	while(scanf(" %s %s", str1, str2) == 2)
	{
		if(strcmp(str1, "add") == 0 && strcmp(str2, "key") == 0)
		{
			scanf(" %s %[^\n]", str1, str2);
			addKey(str1, str2, &l);
		}
		else if(strcmp(str1, "dump") == 0 && strcmp(str2, "keys") == 0)
		{
			dumpKeys(&l);
		}
		else if(strcmp(str1, "tag") == 0 && strcmp(str2, "hit") == 0)
		{
			scanf(" %s %[^\n]", str1, str2);
			tagHit(str1, str2, &t, &l);
		}
		else if(strcmp(str1, "dump") == 0 && strcmp(str2, "tags") == 0)
		{
			dumpTags(&t);
		}
		else if(strcmp(str1, "show") == 0 && strcmp(str2, "tagcontent") == 0)
		{
			scanf(" %s", str2);
			showTag(&t, str2, &l);
		}
		else if(strcmp(str1, "new") == 0 && strcmp(str2, "day") == 0)
		{
			newDay(&t, &l);
		}
		else if(strcmp(str1, "list") == 0 && strcmp(str2, "trending") == 0)
		{
			int xx;
			scanf (" %s %d", str1, &xx);
			if(strcmp(str1, "top") == 0)
                listTop(&t, xx);
            else
            	listBot(&t, xx);
		}
	}
}