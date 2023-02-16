import React from "react";
import { Link } from "react-router-dom";

import styles from "../styles/Main.module.css";

const Main = () => {
    return(
        <div className={styles.wrap}>
            <div className={styles.container}>
                <header className={styles.header}>
                    <nav className={styles.menu}>
                        <Link to="/">Main</Link>
                        <Link to="/static">Static</Link>
                        <Link to="/review">Review</Link>
                    </nav>
                </header>
            </div>
        </div>
    ) 
};

export default Main;