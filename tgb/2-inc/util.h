/**
 *
 * Copyright (c) 2015-2020  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: Header file for the util.h API or library.
 *
 * @features			:
 *
 * @author       		: kernel <kernel_sa@163.com> 
 * 
 * @version      		: V 1.0.0
 *
 * @date         		: 2021年2月22日
 *
 * @environment  		: null
 *
 * @description  		: null 
 *
 * @module       		: null
 *
 * \n <b> @NOTE </b> 	: null
 *
 * \b @Warning      		: null
 * 
 * *
 * Change Logs:
 * Date           author       notes
 * 2021年2月22日		  kernel       create this file
 * 
 * 
 */
 
/**
 * \addtogroup dev module
 * @{
 */


/**
 * \defgroup module API
 *
 * The xxx API module defines a set of functions that a xxx device
 * driver must implement.
 *
 * @{
 */
 
#ifndef UTIL_H_
#define UTIL_H_

/**
 ***************** include file ***************
 * @description		: null 
 * @note			: null 
 * @note			: null 
 */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>


/**
 * @addtogroup group name 
 */

/**@{*/


/**@}*/

/** @formatter:off  */
/**
 ****************<Public macro>****************
 * @description		: null 
 * @note			: null 
 * @note			: null 
 */

#define BIT(x)				(1 << (x)) 			/**< major version number */


/**
 * macro define	- description 
 * @xxx:	description.
 */
#define ARRAY_SIZE(x) 																\
								(sizeof(x) / sizeof((x)[0]))

#define DIV_ROUND_UP(x,y) 															\
								(((x) + (y) - 1) / (y))

#define DIV_ROUND_CLOSEST(x, y) 													\
								(((x) + (y) / 2) / (y))

#define DIV_ROUND_CLOSEST_ULL(x, y) 											\
								DIV_ROUND_CLOSEST(x, y)

#define min(x, y) 																	\
								(((x) < (y)) ? (x) : (y))

#define min_t(type, x, y) 															\
								(type)min((type)(x), (type)(y))

#define max(x, y) 																	\
								(((x) > (y)) ? (x) : (y))

#define max_t(type, x, y) 															\
								(type)max((type)(x), (type)(y))

#define clamp(val, min_val, max_val) 											\
								(max(min((val), (max_val)), (min_val)))

#define clamp_t(type, val, min_val, max_val) 								\
								(type)clamp((type)(val), (type)(min_val), (type)(max_val))

#define abs(x) 																		\
								((x) < 0 ? (-(x)) : (x))

#define swap(x, y) 																	\
								{typeof(x) _tmp_ = (x); (x) = (y); (y) = _tmp_;}

#define round_up(x,y) 																\
								(((x)+(y)-1)/(y))

#define BITS_PER_LONG 32

#define GENMASK(h, l) ({ 															\
								uint32_t t = (uint32_t)(~0UL);					\
								t = t << (BITS_PER_LONG - (h - l + 1));		\
								t = t >> (BITS_PER_LONG - (h + 1));				\
								t;															\
})

#define bswap_constant_32(x) 																			\
								((((x) & 0xff000000) >> 24) | (((x) & 0x00ff0000) >>  8) | 	\
								(((x) & 0x0000ff00) <<  8) | (((x) & 0x000000ff) << 24))

#define U16_MAX			((uint16_t)~0U)

#define S16_MAX			((int16_t)(U16_MAX>>1))

#define DIV_U64(x, y) 	(x / y)

#define UNUSED_PARAM(x) ((void)x)



/**
 ***************<Public typedef>***************
 * @description		: null 
 * @note			: null 
 * @note			: null 
 */


/**
 **************<Public variables>**************
 * @description		: null 
 * @note			: null 
 * @note			: null  
 */

/**
 **************<Public functions>**************
 * @description		: null 
 * @note			: null 
 * @note			: null 
 */

/** @formatter:on */

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @addtogroup group name 
 */

/**@{*/


/**@}*/


/**
 **************<Public functions>**************
 * @description		: null 
 * @note			: null 
 * @note			: null 
 */

/**
 * @brief Find first set bit in word
 * *
 * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月22日
 * @create time  : 2021年2月22日
 * *
 * Parameters
 * *
 * @param	word	new node to be inserted
 *
 * * 
 * @return 
 *			- xxx   description or none
 *			- xxx   description ,the operation status or on successful
 * @retval
 *			- xxx   description
 * *
 * @see  reference @file
 *
 * @note 			description
 *							
 * @warning			description 
 *	
 * Change Logs:
 * Date           author       notes
 * 2021年2月22日		  kernel       create this class
 *						
 */
 uint32_t find_first_set_bit(uint32_t word);

 /**
  * @brief Find last set bit in word.
  * *
  * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月22日
  * @create time  : 2021年2月22日
  * *
  * Parameters
  * *
  * @param	param	new node to be inserted
  *
  * @param [ in or out ]	param	new node to be inserted
  *
  * *
  * @return
  *			- xxx   description or none
  *			- xxx   description ,the operation status or on successful
  * @retval
  *			- xxx   description
  * *
  * @see  reference @file
  *
  * @note 			description
  *
  * @warning			description
  *
  * Change Logs:
  * Date           author       notes
  * 2021年2月22日		  kernel       create this class
  *
  */
 uint32_t find_last_set_bit(uint32_t word);

 /**
  * @brief  Locate the closest element in an array.
  * *
  * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月22日
  * @create time  : 2021年2月22日
  * *
  * Parameters
  * *
  * @param	param	new node to be inserted
  *
  * @param [ in or out ]	param	new node to be inserted
  *
  * *
  * @return
  *			- xxx   description or none
  *			- xxx   description ,the operation status or on successful
  * @retval
  *			- xxx   description
  * *
  * @see  reference @file
  *
  * @note 			description
  *
  * @warning			description
  *
  * Change Logs:
  * Date           author       notes
  * 2021年2月22日		  kernel       create this class
  *
  */
 uint32_t find_closest(int32_t val, const int32_t *array, uint32_t size);
 /**
  * @brief Shift the value and apply the specified mask.
  * *
  * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月22日
  * @create time  : 2021年2月22日
  * *
  * Parameters
  * *
  * @param	param	new node to be inserted
  *
  * @param [ in or out ]	param	new node to be inserted
  *
  * *
  * @return
  *			- xxx   description or none
  *			- xxx   description ,the operation status or on successful
  * @retval
  *			- xxx   description
  * *
  * @see  reference @file
  *
  * @note 			description
  *
  * @warning			description
  *
  * Change Logs:
  * Date           author       notes
  * 2021年2月22日		  kernel       create this class
  *
  */
 uint32_t field_prep(uint32_t mask, uint32_t val);
 /**
  * @brief Get a field specified by a mask from a word.
  * *
  * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月22日
  * @create time  : 2021年2月22日
  * *
  * Parameters
  * *
  * @param	param	new node to be inserted
  *
  * @param [ in or out ]	param	new node to be inserted
  *
  * *
  * @return
  *			- xxx   description or none
  *			- xxx   description ,the operation status or on successful
  * @retval
  *			- xxx   description
  * *
  * @see  reference @file
  *
  * @note 			description
  *
  * @warning			description
  *
  * Change Logs:
  * Date           author       notes
  * 2021年2月22日		  kernel       create this class
  *
  */
 uint32_t field_get(uint32_t mask, uint32_t word);

 /**
  * @brief Log base 2 of the given number.
  * *
  * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月22日
  * @create time  : 2021年2月22日
  * *
  * Parameters
  * *
  * @param	param	new node to be inserted
  *
  * @param [ in or out ]	param	new node to be inserted
  *
  * *
  * @return
  *			- xxx   description or none
  *			- xxx   description ,the operation status or on successful
  * @retval
  *			- xxx   description
  * *
  * @see  reference @file
  *
  * @note 			description
  *
  * @warning			description
  *
  * Change Logs:
  * Date           author       notes
  * 2021年2月22日		  kernel       create this class
  *
  */
 int32_t log_base_2(uint32_t x);
 /**
  * @brief Find greatest common divisor of the given two numbers.
  * *
  * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月22日
  * @create time  : 2021年2月22日
  * *
  * Parameters
  * *
  * @param	param	new node to be inserted
  *
  * @param [ in or out ]	param	new node to be inserted
  *
  * *
  * @return
  *			- xxx   description or none
  *			- xxx   description ,the operation status or on successful
  * @retval
  *			- xxx   description
  * *
  * @see  reference @file
  *
  * @note 			description
  *
  * @warning			description
  *
  * Change Logs:
  * Date           author       notes
  * 2021年2月22日		  kernel       create this class
  *
  */
 uint32_t greatest_common_divisor(uint32_t a,uint32_t b);
 /**
  * @brief Calculate best rational approximation for a given fraction.
  * *
  * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月22日
  * @create time  : 2021年2月22日
  * *
  * Parameters
  * *
  * @param	param	new node to be inserted
  *
  * @param [ in or out ]	param	new node to be inserted
  *
  * *
  * @return
  *			- xxx   description or none
  *			- xxx   description ,the operation status or on successful
  * @retval
  *			- xxx   description
  * *
  * @see  reference @file
  *
  * @note 			description
  *
  * @warning			description
  *
  * Change Logs:
  * Date           author       notes
  * 2021年2月22日		  kernel       create this class
  *
  */
 void rational_best_approximation(uint32_t given_numerator,
 				 uint32_t given_denominator,
 				 uint32_t max_numerator,
 				 uint32_t max_denominator,
 				 uint32_t *best_numerator,
 				 uint32_t *best_denominator);

 /**
  * @brief Calculate the number of set bits.
  * *
  * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月22日
  * @create time  : 2021年2月22日
  * *
  * Parameters
  * *
  * @param	param	new node to be inserted
  *
  * @param [ in or out ]	param	new node to be inserted
  *
  * *
  * @return
  *			- xxx   description or none
  *			- xxx   description ,the operation status or on successful
  * @retval
  *			- xxx   description
  * *
  * @see  reference @file
  *
  * @note 			description
  *
  * @warning			description
  *
  * Change Logs:
  * Date           author       notes
  * 2021年2月22日		  kernel       create this class
  *
  */
 uint32_t hweight8(uint32_t word);

 /**
  * @brief Calculate the quotient and the remainder of an integer division.
  * *
  * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月22日
  * @create time  : 2021年2月22日
  * *
  * Parameters
  * *
  * @param	param	new node to be inserted
  *
  * @param [ in or out ]	param	new node to be inserted
  *
  * *
  * @return
  *			- xxx   description or none
  *			- xxx   description ,the operation status or on successful
  * @retval
  *			- xxx   description
  * *
  * @see  reference @file
  *
  * @note 			description
  *
  * @warning			description
  *
  * Change Logs:
  * Date           author       notes
  * 2021年2月22日		  kernel       create this class
  *
  */
 uint64_t do_div(uint64_t* n,uint64_t base);
 /**
  * @brief Unsigned 64bit divide with 64bit divisor and remainder
  * *
  * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月22日
  * @create time  : 2021年2月22日
  * *
  * Parameters
  * *
  * @param	param	new node to be inserted
  *
  * @param [ in or out ]	param	new node to be inserted
  *
  * *
  * @return
  *			- xxx   description or none
  *			- xxx   description ,the operation status or on successful
  * @retval
  *			- xxx   description
  * *
  * @see  reference @file
  *
  * @note 			description
  *
  * @warning			description
  *
  * Change Logs:
  * Date           author       notes
  * 2021年2月22日		  kernel       create this class
  *
  */
 uint64_t div64_u64_rem(uint64_t dividend, uint64_t divisor, uint64_t *remainder);
 /**
  * @brief Unsigned 64bit divide with 32bit divisor with remainder
  * *
  * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月22日
  * @create time  : 2021年2月22日
  * *
  * Parameters
  * *
  * @param	param	new node to be inserted
  *
  * @param [ in or out ]	param	new node to be inserted
  *
  * *
  * @return
  *			- xxx   description or none
  *			- xxx   description ,the operation status or on successful
  * @retval
  *			- xxx   description
  * *
  * @see  reference @file
  *
  * @note 			description
  *
  * @warning			description
  *
  * Change Logs:
  * Date           author       notes
  * 2021年2月22日		  kernel       create this class
  *
  */
 uint64_t div_u64_rem(uint64_t dividend, uint32_t divisor, uint32_t *remainder);


 /**
  * @brief  Unsigned 64bit divide with 32bit divisor
  * *
  * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月22日
  * @create time  : 2021年2月22日
  * *
  * Parameters
  * *
  * @param	param	new node to be inserted
  *
  * @param [ in or out ]	param	new node to be inserted
  *
  * *
  * @return
  *			- xxx   description or none
  *			- xxx   description ,the operation status or on successful
  * @retval
  *			- xxx   description
  * *
  * @see  reference @file
  *
  * @note 			description
  *
  * @warning			description
  *
  * Change Logs:
  * Date           author       notes
  * 2021年2月22日		  kernel       create this class
  *
  */
 uint64_t div_u64(uint64_t dividend, uint32_t divisor);

 /**
  * @brief Converts from string to int32_t
  * *
  * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月22日
  * @create time  : 2021年2月22日
  * *
  * Parameters
  * *
  * @param	param	new node to be inserted
  *
  * @param [ in or out ]	param	new node to be inserted
  *
  * *
  * @return
  *			- xxx   description or none
  *			- xxx   description ,the operation status or on successful
  * @retval
  *			- xxx   description
  * *
  * @see  reference @file
  *
  * @note 			description
  *
  * @warning			description
  *
  * Change Logs:
  * Date           author       notes
  * 2021年2月22日		  kernel       create this class
  *
  */
 int32_t str_to_int32(const char *str);

 /**
  * @brief Converts from string to uint32_t
  * *
  * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月22日
  * @create time  : 2021年2月22日
  * *
  * Parameters
  * *
  * @param	param	new node to be inserted
  *
  * @param [ in or out ]	param	new node to be inserted
  *
  * *
  * @return
  *			- xxx   description or none
  *			- xxx   description ,the operation status or on successful
  * @retval
  *			- xxx   description
  * *
  * @see  reference @file
  *
  * @note 			description
  *
  * @warning			description
  *
  * Change Logs:
  * Date           author       notes
  * 2021年2月22日		  kernel       create this class
  *
  */
 uint32_t srt_to_uint32(const char *str);
 uint16_t sum_check_16(uint8_t *data, uint16_t crclen);
 uint16_t modbus_crc(uint8_t *data, uint16_t size);


/** @} */ //\defgroup module API
/** @} */ //\addtogroup dev module

#ifdef __cplusplus
}
#endif  /* end of __cplusplus */

#endif /* UTIL_UTIL_H_ */

