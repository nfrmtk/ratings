import React from 'react';
import axios from 'axios';

import styles from "../styles/Static.module.css";

export default class PersonList extends React.Component {
  state = {
    persons: []
  }

  componentDidMount() {
    axios.get(`v1/get-reviews`)
      .then(res => {
        const persons = res.data;
        this.setState({ persons });
      })
  }

  render() {
    return (
      <div className={styles.wrap}>
        <div className={styles.container}>
          <div className={styles.logo}>
           <h1 className={styles.heading}>Ratting game</h1></div>
            <div className={styles.staic}>
              {this.state.persons
                .map(person =>
                  <>
                  <div>
                    <div>{person.username}</div>
                    <div>{person.game}</div>
                    <div>{person.rating}</div>
                    <div>{person.text}</div>
                  </div>
                  </>
                )
              }
            </div>
          </div>
         </div>
        )
      }
}