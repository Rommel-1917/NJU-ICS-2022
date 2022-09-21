#include "cpu/cpu.h"
void set_CF_add(uint32_t result, uint32_t src, size_t data_size) {
	result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)), data_size);
	src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
	cpu.eflags.CF = result < src; 
}
void set_CF_adc(uint32_t result, uint32_t src, size_t data_size) {
    if(cpu.eflags.CF ==0)
    set_CF_add(result, src, data_size);
    else
    {
	result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)), data_size);
	src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
	cpu.eflags.CF = result <= src; 
    }
}

void set_ZF(uint32_t result, size_t data_size) {
	result = result & (0xFFFFFFFF >> (32 - data_size));
	cpu.eflags.ZF = (result == 0);
}


void set_SF(uint32_t result, size_t data_size) {
	result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)), data_size);
	cpu.eflags.SF = sign(result);
}

void set_PF(uint32_t result) { 
    result=result& (0xFFFFFFFF >> 24);
    int count=0;
    while(result!=0)
    {
        if(result%2==1)
        {
            count++;
            result/=2;
        }
        else
        {
            result/=2;
        }
    }
    if(count==0||count==2||count==4||count==6||count==8)
    cpu.eflags.PF=1;
    else
    cpu.eflags.PF=0;
    } 
void set_OF_add(uint32_t result, uint32_t src, uint32_t dest, size_t data_size) {
	switch(data_size) {
		case 8: 
			result = sign_ext(result & 0xFF, 8); 
			src = sign_ext(src & 0xFF, 8); 
			dest = sign_ext(dest & 0xFF, 8); 
			break;
		case 16: 
			result = sign_ext(result & 0xFFFF, 16); 
			src = sign_ext(src & 0xFFFF, 16); 
			dest = sign_ext(dest & 0xFFFF, 16); 
			break;
		default: break;// do nothing
	}
	if(sign(src) == sign(dest)) {
		if(sign(src) != sign(result))
			cpu.eflags.OF = 1;
		else
			cpu.eflags.OF = 0;
	} else {
		cpu.eflags.OF = 0;
	}
}

uint32_t alu_add(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_add(src, dest, data_size);
#else
uint32_t res = 0;
	res = dest + src;                  

	set_CF_add(res, src, data_size);  
	set_PF(res);
	// set_AF();                       
	set_ZF(res, data_size);
	set_SF(res, data_size);
	set_OF_add(res, src, dest, data_size);

	return res & (0xFFFFFFFF >> (32 - data_size)); 
	
#endif
}

uint32_t alu_adc(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_adc(src, dest, data_size);
#else
uint32_t res = 0;
	res = dest + src+cpu.eflags.CF;                  

	set_CF_adc(res, src, data_size);  
	set_PF(res);
	// set_AF();                       
	set_ZF(res, data_size);
	set_SF(res, data_size);
	set_OF_add(res, src, dest, data_size);

	return res & (0xFFFFFFFF >> (32 - data_size)); 
	
#endif
}

uint32_t alu_sub(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sub(src, dest, data_size);
#else
	uint32_t res = 0;
	res = dest - src;                  
uint32_t x=src,y=dest,z=res;
x = sign_ext(x & (0xFFFFFFFF >> (32 - data_size)), data_size);
y = sign_ext(y & (0xFFFFFFFF >> (32 - data_size)), data_size);
z = sign_ext(z & (0xFFFFFFFF >> (32 - data_size)), data_size);
    if(sign(y)==sign(x))
    {
        if(x>y)
        {
            cpu.eflags.CF=1;
        }
	    else{
	        cpu.eflags.CF=0;
	    }
	  cpu.eflags.OF=0;
	}
	else
	{
	    if(x>y)
        {
            cpu.eflags.CF=1;
        }
	    else{
	        cpu.eflags.CF=0;
	    }
	    if(sign(z)==sign(x))
	    cpu.eflags.OF=1;
	    else
	    cpu.eflags.OF=0;
	}
	set_PF(res);
	// set_AF();                       
	set_ZF(res, data_size);
	set_SF(res, data_size);

	return res & (0xFFFFFFFF >> (32 - data_size)); 
#endif
}

uint32_t alu_sbb(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sbb(src, dest, data_size);
#else
	uint32_t res = 0;
	res = dest - src -cpu.eflags.CF;                  
uint32_t x=src,y=dest,z=res;
x = sign_ext(x & (0xFFFFFFFF >> (32 - data_size)), data_size);
y = sign_ext(y & (0xFFFFFFFF >> (32 - data_size)), data_size);
z = sign_ext(z & (0xFFFFFFFF >> (32 - data_size)), data_size);
if(cpu.eflags.CF==0)
{
    if(sign(y)==sign(x))
    {
        if(x>y)
        {
            cpu.eflags.CF=1;
        }
	    else{
	        cpu.eflags.CF=0;
	    }
	  cpu.eflags.OF=0;
	}
	else
	{
	    if(x>y)
        {
            cpu.eflags.CF=1;
        }
	    else{
	        cpu.eflags.CF=0;
	    }
	    if(sign(z)==sign(x))
	    cpu.eflags.OF=1;
	    else
	    cpu.eflags.OF=0;
	}
}
else
{
        if(sign(y)==sign(x))
    {
        if(x>=y)
        {
            cpu.eflags.CF=1;
        }
	    else{
	        cpu.eflags.CF=0;
	    }
	  cpu.eflags.OF=0;
	}
	else
	{
	    if(x>=y)
        {
            cpu.eflags.CF=1;
        }
	    else{
	        cpu.eflags.CF=0;
	    }
	    if(sign(z)==sign(x))
	    cpu.eflags.OF=1;
	    else
	    cpu.eflags.OF=0;
	}
}


	set_PF(res);
	// set_AF();                       
	set_ZF(res, data_size);
	set_SF(res, data_size);

	return res & (0xFFFFFFFF >> (32 - data_size)); 
#endif
}

uint64_t alu_mul(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_mul(src, dest, data_size);
#else
	uint64_t z;
	uint64_t x=dest &(0xFFFFFFFF>>(32-data_size));
	uint64_t y=src &(0xFFFFFFFF>>(32-data_size));
    z=x*y;
    if(z>>data_size!=0)
    {
        cpu.eflags.CF=1;
        cpu.eflags.OF=1;
    }
    else
    {
        cpu.eflags.CF=0;
        cpu.eflags.OF=0;
    }
   

   return z ;
#endif
}

int64_t alu_imul(int32_t src, int32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_imul(src, dest, data_size);
#else
    
	int64_t z=0;
	
    if(data_size==32)
    {
        z=(int64_t)dest*(int64_t)src;
    }
    else
    {
        z=dest*src;
    }
return z;
#endif
}

// need to implement alu_mod before testing
uint32_t alu_div(uint64_t src, uint64_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_div(src, dest, data_size);
#else
    uint64_t x=dest &(0xFFFFFFFFFFFFFFFF>>(64-data_size));
	uint64_t y=src &(0xFFFFFFFFFFFFFFFF>>(64-data_size));
    uint32_t res=x/y;
    return res&(0xFFFFFFFF>>(32-data_size));
    
#endif
}

// need to implement alu_imod before testing
int32_t alu_idiv(int64_t src, int64_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_idiv(src, dest, data_size);
#else
    int64_t x=dest ;
	int64_t y=src ;
    int32_t res=x/y;
    return res;
    
#endif
}

uint32_t alu_mod(uint64_t src, uint64_t dest)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_mod(src, dest);
#else
    uint32_t res=dest%src;
    return res;
#endif
}

int32_t alu_imod(int64_t src, int64_t dest)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_imod(src, dest);
#else
int32_t res=dest%src;
    return res;
#endif
}

uint32_t alu_and(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_and(src, dest, data_size);
#else

uint32_t res=dest&src;

	set_PF(res);
	// set_AF();                       
	set_ZF(res, data_size);
	set_SF(res, data_size);
	cpu.eflags.OF=0;
	cpu.eflags.CF=0;
	return res & (0xFFFFFFFF >> (32 - data_size)); 
#endif
}

uint32_t alu_xor(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_xor(src, dest, data_size);
#else
uint32_t res=dest^src;

	set_PF(res);
	// set_AF();                       
	set_ZF(res, data_size);
	set_SF(res, data_size);
	cpu.eflags.OF=0;
	cpu.eflags.CF=0;
	return res & (0xFFFFFFFF >> (32 - data_size)); 
#endif
}

uint32_t alu_or(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_or(src, dest, data_size);
#else
	uint32_t res=dest|src;

	set_PF(res);
	// set_AF();                       
	set_ZF(res, data_size);
	set_SF(res, data_size);
	cpu.eflags.OF=0;
	cpu.eflags.CF=0;
	return res & (0xFFFFFFFF >> (32 - data_size)); 
#endif
}

uint32_t alu_shl(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_shl(src, dest, data_size);
#else
    for(int i=0;i<src;i++)
    {
        cpu.eflags.CF=sign(sign_ext(dest&(0xFFFFFFFF>>(32 - data_size)), data_size));
        dest<<=1;
        
    }
	set_PF(dest);
	// set_AF();                       
	set_ZF(dest, data_size);
	set_SF(dest, data_size);
	return dest & (0xFFFFFFFF >> (32 - data_size)); 
#endif
}

uint32_t alu_shr(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_shr(src, dest, data_size);
#else
   

    for(int i=0;i<src;i++)
    {
     
        cpu.eflags.CF=dest&1;
        dest>>=1;
        
    }
    dest &= (0xFFFFFFFF>>(32-(data_size-src)));
	set_PF(dest);
	// set_AF();                       
	set_ZF(dest, data_size);
	set_SF(dest, data_size);
	return dest & (0xFFFFFFFF >> (32 - data_size)); 
#endif
}

uint32_t alu_sar(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sar(src, dest, data_size);
#else
    dest=sign_ext(dest,data_size);
    
  if(sign(dest)!=sign(src))
  {
    for(int i=0;i<src;i++)
    {
     
        cpu.eflags.CF=dest&1;
        dest>>=1;
        dest|=0x80000000;
    }
  }
  else
  {
         for(int i=0;i<src;i++)
    {
     
        cpu.eflags.CF=dest&1;
        dest>>=1;
        
    }
  }
	set_PF(dest);
	// set_AF();                       
	set_ZF(dest, data_size);
	set_SF(dest, data_size);
	return dest & (0xFFFFFFFF >> (32 - data_size)); 
#endif
}

uint32_t alu_sal(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sal(src, dest, data_size);
#else
    for(int i=0;i<src;i++)
    {
        cpu.eflags.CF=sign(sign_ext(dest&(0xFFFFFFFF>>(32 - data_size)), data_size));
        dest<<=1;
        
    }
	set_PF(dest);
	// set_AF();                       
	set_ZF(dest, data_size);
	set_SF(dest, data_size);
	return dest & (0xFFFFFFFF >> (32 - data_size)); 
#endif
}
