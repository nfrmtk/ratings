import React from "react";
import { Routes, Route } from "react-router-dom";

import Main from "./Main";
import Static from "./Static";
import Review from "./Review";

const AppRoutes = () => (
  <Routes>
    <Route path="/" element={<Main />} />
    <Route path="/static" element={<Static />} />
    <Route path="/review" element={<Review />} />
  </Routes>
);

export default AppRoutes;
