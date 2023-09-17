#include "ext.h"			// standard Max include, always required (except in Jitter)
#include "ext_obex.h"		// required for "new" style objects
#include "z_dsp.h"			// required for MSP objects

typedef struct {
	t_pxobject super;
    double a;
    double b;
    double r;
    double  * buffer;
} t_keppler;

static t_class *keppler_class = NULL;

void *keppler_new(t_symbol *s, double a, double b) {
    if ( fabs(a) < fabs(b) ) {
        error("Arg1 (%lf) should be greater than Arg2 (%lf)", a, b);
        return NULL;
    } else {
        t_keppler*const this = (t_keppler*const)object_alloc(keppler_class);
        if ( this ) {
            z_dsp_setup((t_pxobject*const)this, 2);
            outlet_new(this, "signal");
            this->r = 0;
            this->a = fabs(a);
            this->b = fabs(b);
            this->buffer = NULL;
        }
        return this;
    }
}

void keppler_free(t_keppler*const this) {
    if (this->buffer)
        sysmem_freeptr(this->buffer);
}

void keppler_radius(t_keppler*const this, t_object const*const dsp64, double**i, long const ni, double**o, long const no, long const count, long const flags, void*userparam) {
    double const sr = (uintptr_t const)userparam;
    for ( register long k = 0, K = count ; k < K ; ++ k ) {
        
    }
}



void keppler_perform64(t_keppler*const this, t_object *dsp64, double **i, long ni, double **o, long no, long sampleframes, long flags, void *userparam) {
}

void keppler_dsp64(t_keppler*const this, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags) {
    post("%d, %d, %d", count[0], count[1], count[2]);
	object_method(dsp64, gensym("dsp_add64"), this, keppler_perform64, 0, NULL);
}

void ext_main(void *r)
{
    keppler_class = (t_class*const)class_new("keppler~", (method)keppler_new, (method)keppler_free, (long)sizeof(t_keppler), 0L, A_FLOAT, A_FLOAT, 0);
    if (keppler_class) {
        class_addmethod(keppler_class, (method)keppler_dsp64, "dsp64", A_CANT, 0);
        class_dspinit(keppler_class);
        class_register(CLASS_BOX, keppler_class);
    }
}
