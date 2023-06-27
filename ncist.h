// This is the header that the plugins also use
// It defines the general API and common macros

#define	LC(c)		('a' <= c && c <= 'z')
#define	UC(c)		('A' <= c && c <= 'Z')
#define	DIG(c)		('0' <= c && c <= '9')

#define	LF	'\n'
#define	BS	0x7F

#define NAMESET(c)	(LC(c) | UC(c) | DIG(c) | (c) == '_' | (c) == ' ')

// TODO: instead of strncmp use fucking strchr
// TODO: most of these should be in a common header
#define CMD(x, cmd)	(args = (x) + sizeof(cmd), \
                    strncmp(x, cmd, sizeof(cmd)-1) == 0)


// TODO: (meta) define the keywords used in the comments TODO, FUTURE, MAYBE 

