import React from "react";
import axios from 'axios';
import { useState } from "react";
import { Link } from "react-router-dom";

import styles from "../styles/Review.module.css";

const FIELDS = {
  USERNAME: "username",
  GAME: "game",
  RATING: "rating",
  REVIEW: "text",
};

const Main = () => {
  const { USERNAME, GAME, RATING, REVIEW, } = FIELDS;

  const [values, setValues] = useState({ [USERNAME]: "", [GAME]: "", [RATING]: "", [REVIEW]: "" });

  const handleChange = ({ target: { value, name } }) => {
    setValues({ ...values, [name]: value });
  };

  const handleClick = (e) => {
    const isDisabled = Object.values(values).some((v) => !v);

    if (isDisabled) {
      e.preventDefault();
    }
    else{
      const user = values;
       
    axios.post(`https://94.103.91.189:8081`, { user })
    .then(res => {
      alert("Спасибо за ваш отзыв");
      console.log(res);            //Delet
      console.log(res.data);
    })
    }
  };
  console.log(values);
  
  return (
    <div className={styles.wrap}>
      <div className={styles.container}>
        <h1 className={styles.heading}>Game review</h1>

        <form className={styles.form}>
          <div className={styles.group}>
            <input
              type="textarea"
              name="username"
              value={values[USERNAME]}
              placeholder="Userame"
              className={styles.input}
              onChange={handleChange}
              autoComplete="off"
              required
            />
          </div>
          <div className={styles.group}>
            <input
              type="textarea"
              name="game"
              placeholder="Game"
              value={values[GAME]}
              className={styles.input}
              onChange={handleChange}
              autoComplete="off"
              required
            />
          </div>
          <div className={styles.group}>
            <input
              type="textarea"
              name="rating"
              placeholder="Rating"
              value={values[RATING]}
              className={styles.input}
              onChange={handleChange}
              autoComplete="off"
              required
            />
          </div>
          <div className= {styles.group__rewiew}>
            <textarea
            rows="4"
              name="text"
              placeholder="Review"
              value={values[REVIEW]}
              className={styles.textarea}
              onChange={handleChange}
              autoComplete="off"
              required
            ></textarea>
          </div>
          <Link
            className={styles.group__button}
            onClick={handleClick}
            to={`/`}
          >
            <button type="submit" className={styles.button}>
            Send
            </button>
          </Link>
        </form>
      </div>
    </div>
  );
};

export default Main;



