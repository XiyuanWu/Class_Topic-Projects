CREATE INDEX idx_users_login ON Users USING btree(login);
CREATE INDEX idx_users_role ON Users USING btree(role);

CREATE INDEX idx_items_name ON Items USING btree(itemName);
CREATE INDEX idx_items_ingredients ON Items USING btree(ingredients);
CREATE INDEX idx_items_typeofitem ON Items USING btree(typeOfItem);
CREATE INDEX idx_items_price ON Items USING btree(price);

CREATE INDEX idx_store_id ON Store USING btree(storeID);
CREATE INDEX idx_store_city ON Store USING btree(city);
CREATE INDEX idx_store_state ON Store USING btree(state);
CREATE INDEX idx_store_reviewscore ON Store USING btree(reviewScore);
CREATE INDEX idx_store_isOpen ON Store USING btree(isOpen);

CREATE INDEX idx_foodorder_login ON FoodOrder USING btree(login);
CREATE INDEX idx_foodorder_storeid ON FoodOrder USING btree(storeID);
CREATE INDEX idx_foodorder_orderid ON FoodOrder USING btree(orderID);
CREATE INDEX idx_foodorder_timestamp ON FoodOrder USING btree(orderTimestamp);
CREATE INDEX idx_foodorder_totalprice ON FoodOrder USING btree(totalPrice);
CREATE INDEX idx_foodorder_status ON FoodOrder USING btree(orderStatus);

CREATE INDEX idx_itemsinorder_orderid ON ItemsInOrder USING btree(orderID);
CREATE INDEX idx_itemsinorder_itemname ON ItemsInOrder USING btree(itemName);
CREATE INDEX idx_itemsinorder_quantity ON ItemsInOrder USING btree(quantity);