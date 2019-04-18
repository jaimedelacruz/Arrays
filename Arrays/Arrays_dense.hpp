#ifndef ARRAYS_DENSE_H
#define ARRAYS_DENSE_H

namespace mem{
  template<typename T, size_t N, typename Rep = internal::Storage<T>>
  class Array{
    using idx_t = typename Traits<T>::index;
  private:
    internal::DDim<idx_t,N> Dimensions;
    Rep Data;
  public:

    // ------------------------------------------------------------------ //

    M_INLINE idx_t  shape(size_t const ii)const{return Dimensions.Dim[ii];}
    M_INLINE idx_t offset(size_t const ii)const{return Dimensions.Off[ii];}
    
    // ------------------------------------------------------------------ //
    
    M_INLINE Rep const &getStorage()const
      {return *static_cast<Rep const*>(&Data);}

    // ------------------------------------------------------------------ //

    M_INLINE internal::DDim<idx_t,N> const& getDimensions()const
      {return *static_cast<internal::DDim<idx_t,N> const*>(&Dimensions);}

    // ------------------------------------------------------------------ //

    template<typename ...S> M_INLINE explicit Array(const S... xx):
    Dimensions(xx...), Data(Dimensions.size()){};

    // ------------------------------------------------------------------ //

    M_INLINE Array(Array<T,N> const &in):
    Dimensions(in.getDimensions()), Data(in.getStorage()){};
    
    // ------------------------------------------------------------------ //

    M_INLINE explicit Array(Rep const& in): Data(in){};

    // ------------------------------------------------------------------ //

    template<typename ...S> M_INLINE explicit Array(T* datain, const S... xx):
      Dimensions(xx...), Data(datain, Dimensions.size()){};
    
    // ------------------------------------------------------------------ //

    template<typename ...S>
    M_INLINE T &operator()(const S... indexes)const
      {
	return Data.getData()
	  [internal::linearize_dimensions<N-1,idx_t,N>::run(Dimensions.getConstRefDim(),
							    Dimensions.getConstRefOff(),
							    {indexes...})];
      }

    
    // ------------------------------------------------------------------ //

    M_INLINE T &operator[](size_t const index){return Data[index];}

    // ------------------------------------------------------------------ //

    M_INLINE decltype(auto) operator[](size_t const ii)const{return Data[ii];}

    // ------------------------------------------------------------------ //

    M_INLINE void Zero()const
    {
      if constexpr(mem::Traits<T>::zero_with_memset)
	Data.Zero();
      else
	this->operator=(static_cast<T>(0));
    }
    
    // ------------------------------------------------------------------ //

    M_INLINE void ZeroMem()const
    {
      Data.Zero();
    }
    
    // ------------------------------------------------------------------ //

    M_INLINE size_t size()const{return static_cast<size_t>(Dimensions.size());}

    // ------------------------------------------------------------------ //

    template<typename ...S>
      M_INLINE void reshape(const S... indexes)
      {
	static_assert((sizeof...(S) == N || sizeof...(S) == 2*N));
	
	internal::DDim<idx_t, N> const nDim(indexes...);
	
	if(nDim.size() != Dimensions.size()){
	  char tmp[100];
	  sprintf(tmp,"attempted array reshape with wrong element number [%ld] != [%ld]",
		  nDim.size(), Dimensions.size());
	  internal::array_error(std::string(tmp), __FUNCTION__);
	}
	
	Dimensions = nDim;
      }
    
    // ------------------------------------------------------------------ //

    Array<T,N,Rep> &operator=(Array<T,N,Rep> const& in)
      {
       Dimensions = in.getDimensions();
       Data = in.getStorage();
       return *this;
      }

    // ------------------------------------------------------------------ //
    
    template<typename T2, typename Rep2>
    Array<T,N,Rep>& operator=(Array<T2, N, Rep2> const& b)
    {
      size_t const nel = Data.size();
      T* __restrict__ iData = Data.getData();
      for(size_t ii=0; ii<nel; ++ii)
	iData[ii] = b[ii];

      return *this;
    }
    
    // ------------------------------------------------------------------ //

    M_INLINE void operator=(T const val)const
      {
       const size_t imax = Data.size();
       T* __restrict__ iData = Data.getData();
       for(size_t ii=0; ii<imax; ++ii) iData[ii] = val;
      }
    
    // ------------------------------------------------------------------ //

    template<typename ...S>
    void resize(const S... indexes)
    {
      static_assert(sizeof...(S) == N || sizeof...(S) == 2*N,
		    "wrong number of indexes");
      
      internal::DDim<idx_t, N>nDim(indexes...);
      
      if(nDim.size() != Dimensions.size())
	Data.allocate(nDim.size());
      
      Dimensions = nDim;
    }

    // ------------------------------------------------------------------ //  

    Rep const& rep()const{return Data;}
    Rep&       rep()     {return Data;}
    
  }; // class Array
}


#endif
