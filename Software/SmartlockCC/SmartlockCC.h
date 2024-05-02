/* Static table to allow some UID */
typedef struct auth {
    uint64_t UID;
    char *name;
} auth_t;

static const auth_t table[] = {
    { 0x0123456789ABCDEF, "Dupont" },
    { 0xFEDCBA9876543210, "Dupond" },
};

#define tablesize sizeof(table) / (sizeof(auth_t))

