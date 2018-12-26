extern void
normalize_path (char *path)
{
  char *path_end_ptr = 0;
  const char PATH_END = '\0';
  for (path_end_ptr = path; *path_end_ptr != PATH_END; ++path_end_ptr);
  char *current_ptr = path, *correct_ptr = path;
  do
  {
    if (!(current_ptr - 1 >= path && *current_ptr == '/'
          && *(current_ptr - 1) == '/'))
    {
      *(correct_ptr++) = *current_ptr;
    }
    ++current_ptr;
  }
  while (current_ptr - 1 != path_end_ptr);
  current_ptr = correct_ptr = path;
  do
  {
    if (current_ptr - 2 >= path && *(current_ptr - 2) == '.'
        && *(current_ptr - 1) == '.' && *current_ptr == '/')
    {
      int parent_level = 0;
      while (correct_ptr >= path && parent_level < 2)
      {
        if (*(--correct_ptr) == '/')
        {
          ++parent_level;
        }
      }
      ++correct_ptr;
    }
    else if (current_ptr - 1 >= path
             && *(current_ptr - 1) == '.'
             && *current_ptr == '/')
    {
      --correct_ptr;
    }
    else
    {
      *(correct_ptr++) = *current_ptr;
    }
    ++current_ptr;
  }
  while (current_ptr - 1 >= path && *(current_ptr - 1) != PATH_END);
}