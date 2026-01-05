import { StrictMode } from "react";
import { createRoot } from "react-dom/client";
import S01 from "./S01";
import S02 from "./S02";
import S03 from "./S03";
import S04 from "./S04";
import S05 from "./S05";

createRoot(document.getElementById("root")!).render(
  <StrictMode>
    <S01 />
    <S02 />
    <S03 />
    <S04 />
    <S05 />
  </StrictMode>
);
