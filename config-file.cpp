#include <stdio.h>
#include <stdlib.h>
#include "stdafx.h"
#include "config-file.h"
#include "scumm.h"

#define MAXLINELEN 256

static char * ltrim(char * t) {
    for (; *t && (*t == ' '); t++);
    return t;
}

static char * rtrim(char * t) {
    int l;
    
    for (l = strlen(t) - 1; l; l--) {
	if (t[l] == ' ') {
	    *t = 0;
	} else {
	    return t;
	}
    }
    return t;
}

class hashconfig {
  public:
      hashconfig(const char *);
      ~hashconfig();
    const char * get_domain() const;
    bool is_domain(const char *) const;
    const char * get(const char *) const;
    const char * set(const char *, const char *);
    const char * getdomain() const;
    void flush(FILE *) const;
    void rename(const char *);
    void merge(const hashconfig *);
  private:
    char * domain;
    char ** keys, ** values;
    int nkeys;
};

hashconfig::hashconfig(const char * dom) : domain(dom ? Scumm::Strdup(dom) : 0), keys(0), values(0), nkeys(0) { }

hashconfig::~hashconfig() {
    int i;

    Scumm::free(domain);
    for (i = 0; i < nkeys; i++) {
    }
}

const char * hashconfig::get_domain() const {
    return domain;
}

bool hashconfig::is_domain(const char * d) const {
    if (!this)
	return false;
    return d ? domain ? !strcmp(d, domain) ? true : false : false : false;
}

const char * hashconfig::get(const char * key) const {
    int i;
    
    for (i = 0; i < nkeys; i++) {
	if (!strcmp(key, keys[i])) {
	    return values[i];
	}
    }
    
    return 0;
}

const char * hashconfig::set(const char * key, const char * value) {
    int i;
    
    for (i = 0; i < nkeys; i++) {
	if (!strcmp(key, keys[i])) {
	    Scumm::free(values[i]);
	    return values[i] = value ? Scumm::Strdup(value) : 0;
	}
    }
    
    nkeys++;
    keys = (char **) Scumm::realloc(keys, nkeys * sizeof(char *));
    values = (char **) Scumm::realloc(values, nkeys * sizeof(char *));
    keys[nkeys - 1] = Scumm::Strdup(key);
    return values[nkeys - 1] = value ? Scumm::Strdup(value) : 0;
}

const char * hashconfig::getdomain() const {
    return domain;
}

void hashconfig::flush(FILE * cfg_file) const {
    int i;
    
    if (!this || !domain)
	return;

    fprintf(cfg_file, "[%s]\n", domain);
    
    for (i = 0; i < nkeys; i++) {
	if (values[i]) {
	    fprintf(cfg_file, "%s=%s\n", keys[i], values[i]);
	}
    }
    fprintf(cfg_file, "\n");
}

void hashconfig::rename(const char * d) {
    Scumm::free(domain);
    domain = d ? Scumm::Strdup(d) : 0;
}

void hashconfig::merge(const hashconfig * h) {
    int i;
    
    for (i = 0; i < h->nkeys; i++) {
	set(h->keys[i], h->values[i]);
    }
}

// The config-class itself.

Config::Config(const char * cfg, const char * d) : filename(Scumm::Strdup(cfg)), domain(d ? Scumm::Strdup(d) : 0), hash(0), ndomains(0) {
    FILE * cfg_file;
    char t[MAXLINELEN];
    
    if (!(cfg_file = fopen(cfg, "r"))) {
	debug(1, "Unable to open configuration file: %s.\n", filename);
    } else {
	while (!feof(cfg_file)) {
	    if (!fgets(t, MAXLINELEN, cfg_file))
		continue;
	    if (t[0] != '#') {
		if (t[0] == '[') {
		    // It's a new domain which begins here.
		    char * p = strchr(t, ']');
		    if (!p) {
			debug(1, "Config file buggy: no ] at the end of the domain name.\n");
		    } else {
			*p = 0;
			set_domain(t + 1);
		    }
	        } else {
	    	    // It's a new key in the domain.
		    if (!domain) {
			debug(1, "Config file buggy: we have a key without a domain first.\n");
		    }
		    char * p = strchr(t, '\n');
		    if (p)
			*p = 0;
		    p = strchr(t, '\r');
		    if (p)
			*p = 0;
		    
		    if (!(p = strchr(t, '='))) {
			if (strlen(t))
			    debug(1, "Config file buggy: there is junk: %s\n", t);
		    } else {
			char * key, * value;
			*p = 0;
			key = ltrim(rtrim(t));
			value = ltrim(p + 1);
			set(key, value);
		    }
		}
	    }
	}
	set_domain(d);
	fclose(cfg_file);
    }
}

Config::~Config() {
    int i;

    Scumm::free(filename);
    Scumm::free(domain);
    
    for (i = 0; i < ndomains; i++) {
	delete hash[i];
    }
    Scumm::free(hash);    
}

const char * Config::get(const char * key, const char * d) const {
    int i;
    
    if (!d)
	d = domain;
    
    for (i = 0; i < ndomains; i++) {
	if (hash[i]->is_domain(d)) {
	    return hash[i]->get(key);
	}
    }
    
    return 0;
}

const char * Config::set(const char * key, const char * value, const char * d) {
    int i;
    
    if (!d)
	d = domain;
    
    for (i = 0; i < ndomains; i++) {
	if (hash[i]->is_domain(d)) {
	    return hash[i]->set(key, value);
	}
    }
    
    ndomains++;
    hash = (hashconfig **) Scumm::realloc(hash, ndomains * sizeof(hashconfig *));
    hash[ndomains - 1] = new hashconfig(domain);
    
    return hash[ndomains - 1]->set(key, value);
}

const char * Config::set(const char * key, int value_i, const char * d) {
    int i;
    char value[MAXLINELEN];

    sprintf(value, "%i", value_i);
    
    if (!d)
	d = domain;
    
    for (i = 0; i < ndomains; i++) {
	if (hash[i]->is_domain(d)) {
	    return hash[i]->set(key, value);
	}
    }
    
    ndomains++;
    hash = (hashconfig **) Scumm::realloc(hash, ndomains * sizeof(hashconfig *));
    hash[ndomains - 1] = new hashconfig(domain);
    
    return hash[ndomains - 1]->set(key, value);
}

void Config::set_domain(const char * d) {
    int i;
    Scumm::free(domain);
    domain = d ? Scumm::Strdup(d) : 0;

    for (i = 0; i < ndomains; i++) {
	if (hash[i]->is_domain(domain))
	    return;
    }
    ndomains++;
    hash = (hashconfig **) Scumm::realloc(hash, ndomains * sizeof(hashconfig *));
    hash[ndomains - 1] = new hashconfig(domain);
}

void Config::flush() const {
    FILE * cfg_file;
    int i;
    
    if (!(cfg_file = fopen(filename, "w"))) {
	debug(1, "Unable to write configuration file: %s.\n", filename);
    } else {
	for (i = 0; i < ndomains; i++) {
	    hash[i]->flush(cfg_file);
	}
	fclose(cfg_file);
    }
}

void Config::rename_domain(const char * d) {
    int i, index = -1;
    
    for (i = 0; i < ndomains; i++) {
	if (hash[i]->is_domain(d)) {
	    index = i;
	}
    }
    
    for (i = 0; i < ndomains; i++) {
	if (hash[i]->is_domain(domain)) {
	    if (index >= 0) {
		hash[i]->merge(hash[index]);
		hash[index]->rename(0);
		rename_domain(d);
	    } else {
		hash[i]->rename(d);
	    }
	}
    }
}

void Config::delete_domain(const char * d) {
    int i;
    
    for (i = 0; i < ndomains; i++) {
	if (hash[i]->is_domain(d)) {
	    hash[i]->rename(0);
	}
    }
}

void Config::change_filename(const char * f) {
    Scumm::free(filename);
    filename = f ? Scumm::Strdup(f) : 0;
}

void Config::merge_config(const Config * c) {
    int i, j;
    bool found;
    
    for (i = 0; i < c->ndomains; i++) {
	found = false;
	for (j = 0; j < ndomains; j++) {
	    if (hash[j]->is_domain(c->hash[i]->getdomain())) {
		hash[j]->merge(c->hash[i]);
		found = true;
		break;
	    }
	}
	if (!found) {
	    ndomains++;
	    hash = (hashconfig **) Scumm::realloc(hash, ndomains * sizeof(hashconfig *));
	    hash[ndomains - 1] = new hashconfig(domain);
	    hash[ndomains - 1]->merge(c->hash[i]);
	}
    }
}
