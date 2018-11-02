#include <stdio.h>
#include <stdint.h>

typedef struct Person 
{
  uint32_t  id;
  uint8_t   age;
  char      first_name[20];
  char      last_name[20];
} person_t;

extern void
sort_by_age(int N, person_t *persons);

int
main ()
{
  const int N = 6;
  person_t persons[6] =
  {
    {1, 31, "Nick", "Lincoln"},
    {5, 50, "John", "Jackson"},
    {4, 14, "Mick", "Jaggers"},
    {3, 19, "Jack", "Johnson"},
    {2, 37, "Josh", "Unknown"},
    {6, 62, "Tedd", "Lincoln"}
  };
    
  puts("----------------------");
  sort_by_age(N, persons);
  puts("----------------------");
  
  for (int i = 0; i < N; i++)
  {
    printf("\n(%d)\n", persons[i].id);
    printf("\t%d\n\t", persons[i].age);
    puts(persons[i].first_name);
    printf("\t");
    puts(persons[i].last_name);
  }
	
  return 0;
}

