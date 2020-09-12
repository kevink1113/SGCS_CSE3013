class Str {
  private:
    char *str;
    int len;
  public:
    Str(int leng);
    Str(char *neyong);
    ~Str();
    int length(void);
    char *contents(void);
    int compare(class Str &a);
    int compare(char *a);
    void operator=(char *a);
    void operator=(class Str &a);
};
