// include this in the plugin for preamble and declerations

#define callback(...)    _callback(_plugid, __VA_ARGS__)

#define PLUGIN_PREAMBLE(name)   \
    char _name[16] = name;      \

typedef void (*cb_t)(int, char*, ...);

int init(void);

int _plugid;
cb_t _callback;

int _plugin_init(int id, cb_t cb)
{
    _plugid = id;
    _callback = cb;
    return init();
}
