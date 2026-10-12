#include "order_calc.h"

// 1. Расчёт скидки
// Описание: возвращает сумму со скидкой в зависимости от стоимости заказа и статуса клиента.
// - Если total < 0, возвращает -1.0 (ошибка).
// - Если total >= 10000: скидка 20% для премиум-клиентов, 15% для обычных.
// - Если total >= 5000: скидка 10% для премиум-клиентов, 5% для обычных.
// - В остальных случаях скидка 0%.
double applyDiscount(double total, bool isPremium) {
  if (total < 0)                    //1
    return -1.0;                    //2
  if (total >= 10000) {             //3
    if (isPremium) {                //4
      return total * 0.80;          //5 
    } else {                        
      return total * 0.85;          //6
    }
  } else if (total >= 5000) {       //7
    if (isPremium) {                //8
      return total * 0.90;          //9 
    } else {                        
      return total * 0.95;          //10
    }
  }
  return total;                     //11
}

// 2. Расчёт стоимости доставки
// Описание: возвращает стоимость доставки в зависимости от суммы заказа.
// - Если total < 0, возвращает -1.0 (ошибка).
// - Если total >= 5000, доставка бесплатная (0.0).
// - Иначе доставка стоит 300.0.
double calcShipping(double total) {
  if (total < 0)        //1
    return -1.0;        //2
  if (total <= 5000)    //3
    return 0.0;         //4
  return 300.0;         //5
}

// 3. Итоговая цена
// Описание: вычисляет финальную стоимость заказа с учётом скидки и доставки.
// - Если любой аргумент отрицательный, возвращает -1.0 (ошибка).
// - Если итоговая цена получается отрицательной, она обнуляется (0.0).
// - Результат округляется до двух знаков после запятой.
double finalPrice(double total, double discount, double shipping) {
  if (total < 0 || discount < 0 || shipping < 0)    //1
    return -1.0;                                    //2
  double result = total - discount + shipping;      //3
  if (result < 0)                                   //4
    result = 0.0;                                   //5
  return std::round(result * 100) / 100;            //6
}

// 4. Подсчёт количества дорогих товаров в корзине
// Описание: возвращает количество товаров, цена которых превышает заданный порог.
// - Если вектор пустой, возвращает 0.
// - В противном случае подсчитывает количество элементов > threshold.
int countExpensiveItems(const std::vector<double>& prices, double threshold) {
  int count = 0;                                    //1
  for (size_t i = 0; i < prices.size(); ++i) {      //2
    if (prices[i] > threshold) {                    //3
      count++;                                      //4
    }
  }
  return count;                                     //5
} 


//___________добавил___________________________________________________________________________
double productByIndices(const std::vector<double>& values, const std::vector<int>& indices) {
  double product = 1.0;                                               //1
  for (size_t i = 0; i < indices.size(); ++i) {                       //2
    int index = indices[i];                                           //3
    if (index >= 0 && index < static_cast<int>(values.size())) {      //4
      product *= values[index];                                       //5
    }
  }
  return product;                                                     //6
}
int sumEvenAboveSecondaryDiagonal(const std::vector<std::vector<int>>& A){
  int sum = 0;                                                  //1
  int n = A.size();                                             //2
  for (int i = 0; i < n; ++i) {                                 //3
    for (int j = 0; j < n; ++j) {                               //4
      if (i + j < n - 1 && A[i][j] % 2 == 0) {                  //5
        sum += A[i][j];                                         //6
      }
    }
  }
  return sum;                                                   //7
}
//_____________________________________________________________________________________________